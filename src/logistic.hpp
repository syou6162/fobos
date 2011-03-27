#ifndef LOGISTIC_HPP
#define LOGISTIC_HPP

#include "fobos.hpp"

class Logistic : public FOBOS {
public:
  Logistic(const float eta_, const float lambda_) : FOBOS(eta_, lambda_) {};

  void update(const int max_iter) {
	std::random_shuffle(examples.begin(), examples.end());
	for (int iter = 0; iter < max_iter; iter++) {
	  std::vector<std::pair<fv_t, int> >::iterator it = examples.begin();
	  for(; it != examples.end(); it++) {
		fv_t fv = it->first; int y = it->second;
		logistic(fv, y, eta);
	  }
	  l1_regularize(iter);
	  std::cerr << w.size() << std::endl;
	}
  };

  void logistic(const fv_t &fv, const int y, float scale) {
	float inner_product = dotproduct(fv); // w^T x
    for (size_t i = 0; i < fv.size(); i++) {
	  int key = fv[i].first;
      float x_i = fv[i].second;
	  std::tr1::unordered_map<int, float>::iterator wit = w.find(key);
	  float tmp = - y * exp(- y * inner_product) / (1.0 + exp(- y * inner_product)) * x_i;
      if (wit != w.end()) {
		wit->second -= scale * tmp;
      } else {
        w[key] = - scale * tmp;
      }
	}
  };

  float logstic_predict(const fv_t& fv) {
	float inner_product = dotproduct(fv); // w^T x
	return 1.0 / (1.0 + exp(- inner_product));
  };

  bool classify(const fv_t& fv, const int y) {
	if (y > 0) {
	  return logstic_predict(fv) > 0.5;
	} else {
	  return logstic_predict(fv) < 0.5;
	}
  };
};

#endif
