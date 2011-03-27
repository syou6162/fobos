#ifndef SVM_HPP
#define SVM_HPP

#include "fobos.hpp"

class SVM : public FOBOS {
public:
  SVM(const float eta_, const float lambda_) : FOBOS(eta_, lambda_) {};

  float margin(const fv_t &fv, const int y) {
	return dotproduct(fv) * y;
  };
  
  void update(const int max_iter) {
	std::random_shuffle(examples.begin(), examples.end());
	for (int iter = 0; iter < max_iter; iter++) {
	  std::vector<std::pair<fv_t, int> >::iterator it = examples.begin();
	  for(; it != examples.end(); it++) {
		fv_t fv = it->first; int y = it->second;
		if (margin(fv, y) < 1.0) muladd(fv, y, eta);
	  }
	  l1_regularize(iter);
	  std::cerr << w.size() << std::endl;
	}
  };

  bool classify(const fv_t& fv, const int y) {
	return margin(fv, y) > 0.0;
  };
};

#endif
