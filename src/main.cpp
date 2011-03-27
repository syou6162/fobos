#include "svm.hpp"
#include "logistic.hpp"
#include "cmdline.h"

void add_training_example(FOBOS& fobos, const std::string& training_filename) {
  std::vector<fv_t> v;
  std::ifstream ifs(training_filename.c_str());
  while(ifs && !ifs.eof()) {
	std::string line;
	fv_t fv;
	int y;
	getline(ifs, line);
	int ret = parse_line(line, fv, y);
	if (ret == 0) {
	  fobos.add_example(fv, y);
	}
  }
};

void run(FOBOS& fobos, 
		 const std::string& training_filename, 
		 const std::string& test_filename, 
		 const int max_iter) {
  add_training_example(fobos, training_filename);

  std::cerr << "Finished readeing data..." << std::endl;
  fobos.update(max_iter);

  std::ifstream ifs(test_filename.c_str());
  int ok1 = 0, ok2 = 0;
  int ng1 = 0, ng2 = 0;

  while(!ifs.eof()) {
	std::string line;
	fv_t fv;
	int y;
	getline(ifs, line);
	int ret = parse_line(line, fv, y);
	if (ret != 0) continue;
	if (y > 0) {
	  if (fobos.classify(fv, y)) {
		ok1++;
	  } else {
		ng1++;
	  }
	} else {
	  if (fobos.classify(fv, y)) {
		ok2++;
	  } else {
		ng2++;
	  }
	}
  }
  std::cerr << "  accuracy:  " << ((double) ok1 + ok2) / (ok1 + ok2 + ng1 + ng2) << std::endl;
  std::cerr << "  precision: " << ((double) ok1) / (ok1 + ng1) << std::endl;
  std::cerr << "  recall:    " << ((double) ok1) / (ok1 + ng2) << std::endl;
};

int main(int argc, char **argv) {
  cmdline::parser a;
  a.add<std::string>("train", 0, "training filename", true);
  a.add<std::string>("test", 0, "test filename", true);
  a.add<std::string>("classifier", 'c', "classifier type", false, "svm", 
					 cmdline::oneof<std::string>("svm", "logistic"));

  a.add<double>("eta", 'e', "update step", false, 1.0);
  a.add<double>("lambda", 'l', "regularization parameter", false, 0.9);
  a.add<int>("max_iter", 'i', "number of max iteration", false, 10);
  a.add<bool>("vervose", 'v', "vervose option ", false, false);
  a.add("help", 0, "print this message");

  bool ok = a.parse(argc, argv);
  if (argc == 1 || a.exist("help")) {
	std::cerr << a.usage();
    return 0;
  }
  
  if (!ok) {
	std::cerr << a.error() << std::endl << a.usage();
    return 0;
  }

  if (a.get<std::string>("classifier") == "svm") {
	SVM svm(a.get<double>("eta"), a.get<double>("lambda"));
	run(svm, a.get<std::string>("train"), a.get<std::string>("test"), a.get<int>("max_iter"));
  } else {
	Logistic logistic(a.get<double>("eta"), a.get<double>("lambda"));
	run(logistic, a.get<std::string>("train"), a.get<std::string>("test"), a.get<int>("max_iter"));
  }
  return 0;
};
