#ifndef FOBOS_HPP
#define FOBOS_HPP

#include "util.hpp"
#include <tr1/unordered_map>

class FOBOS {
public:
  std::vector<std::pair<fv_t, int> > examples;
  std::tr1::unordered_map<int, float> w;
  int exampleN;
  float eta;
  float lambda;

  FOBOS(const float eta_, const float lambda_) :
    exampleN(0),
    eta(eta_),
    lambda(lambda_)
  {};
  
  float clip_by_zero(const float a, const float b) {
    if (a > 0.0) {
      if (a > b) {
        return a - b;
      } else {
        return 0.0;
      }
    } else {
      if (a < - b) {
        return a + b;
      } else {
        return 0.0;
      }
    }
  };

  float dotproduct(const fv_t& fv) {
    float m = 0.0;
    size_t fv_size = fv.size();
    for (size_t i = 0; i < fv_size; i++) {
	  int key = fv[i].first;
      float x_i = fv[i].second;
	  std::tr1::unordered_map<int, float>::iterator wit = w.find(key);
      if (wit != w.end()) {
        m +=  x_i * wit->second;
      }
    }
    return m;
  };

  void l1_regularize(const int iter) {
	float lambda_hat = get_eta(iter) * lambda;

	std::tr1::unordered_map<int, float> tmp = w;
	std::tr1::unordered_map<int, float>::iterator it = tmp.begin();
	for (; it != tmp.end(); it++) {
	  int key = it->first;
	  std::tr1::unordered_map<int, float>::iterator wit = w.find(key);
	  float aaa = wit->second;
	  wit->second = clip_by_zero(wit->second, lambda_hat);
	  if (fabsf(aaa) < lambda_hat) {
		w.erase(wit);
	  } 
	}
  };

  virtual void update(const int max_iter) = 0;

  void add_example(const fv_t& fv, const int y) {
	exampleN++;
	examples.push_back(std::make_pair(fv, y));
  };

  float get_eta(const int iter) {
	return 1.0 / (1.0 + iter / examples.size());
  };

  void muladd(const fv_t &fv, const int y, float scale) {
    for (size_t i = 0; i < fv.size(); i++) {
	  int key = fv[i].first;
      float x_i = fv[i].second;

	  std::tr1::unordered_map<int, float>::iterator wit = w.find(key);
      if (wit != w.end()) {
		wit->second += y * x_i * scale;
      } else {
        w[key] = y * x_i * scale;
      }
    }
  };

  virtual bool classify(const fv_t& fv, const int y) = 0;
};

#endif
