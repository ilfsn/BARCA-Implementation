#pragma once
#include <vector>
#include <random>


class one_class {

public:

	int CLASSIFY(std::vector<double> xj);
	int UPDATE_MODEL(std::vector<double> xj);
	std::vector<double> Latest;
		
	one_class(double rho, double eta, double nu, double gamma, int dimension) : rho(rho), eta(eta), nu(nu), dimension(dimension), gamma(gamma){
		sigma = std::vector<double>(dimension, 1);
		t = 0;
	};
	
private:
	double rho, eta, nu, gamma;
	std::vector<std::vector<double>> V;
	std::vector<double> alpha;		
	int dimension;
	int t;	
	std::vector<double> mu, sigma;
	std::vector<double> NORMALIZE(std::vector<double> xj);
	int NORM_UPDATE();
};

class binary_class {

public:	
	binary_class(double rho, double eta, double lambda, double gamma, bool leaf, int dimension) : rho(rho), eta(eta), lambda(lambda), leaf(leaf), dimension(dimension), gamma(gamma){
	
		sigma = std::vector<double> (dimension, 1);
		t = 0;
	};

	binary_class* left;
	binary_class* right;
	int CLASSIFYmulti(std::vector<double> xj);	
	int BALANCED_UPDATE(std::vector<double> xj, int yi);
	int CLASSIFY(std::vector<double> xj);
	std::vector<double> Latest;
	int error_code(int left, int right);

private:
	double rho, eta, lambda, gamma;
	bool leaf;
	int dimension;
	int errc_left, errc_righ;
	std::vector<std::vector<double>> V;
	std::vector<double> alpha;
	std::vector<std::vector<double>> P;
	std::vector<std::vector<double>> N;
	int t;	
	std::vector<double> mu, sigma;
	
	int UPDATE_MODEL(std::vector<double> xj, int yj);
	std::vector<double> NORMALIZE(std::vector<double> xj);
	int NORM_UPDATE();
};


class multi_classDLTM {

public:	
	std::vector< binary_class* > D, L, T, M;
	binary_class DM, DL, LM, DT, LT, TM;
	multi_classDLTM(double rho, double eta, double lambda, double  gamma, int dimension) : rho(rho), eta(eta), lambda(lambda), gamma(gamma), dimension(dimension),
		DM (binary_class(rho, eta, lambda, gamma, false, dimension)),
		DL (binary_class(rho, eta, lambda, gamma, false, dimension)),
		LM (binary_class(rho, eta, lambda, gamma, false, dimension)),
		DT (binary_class(rho, eta, lambda, gamma, true, dimension)),
		LT (binary_class(rho, eta, lambda, gamma, true, dimension)),
		TM (binary_class(rho, eta, lambda, gamma, true, dimension))
	{
		DM.left = &DL;
		DM.right = &LM;
		DL.left = &DT;
		DL.right = &LT;
		LM.left = &LT;
		LM.right = &TM;

		DT.error_code(2, 4);
		LT.error_code(3, 4);
		TM.error_code(4, 5);

		
		D.push_back(&DM);
		D.push_back(&DL);
		D.push_back(&DT);

		L.push_back(&DL);
		L.push_back(&LM);
		L.push_back(&LT);

		T.push_back(&DT);
		T.push_back(&LT);
		T.push_back(&TM);

		M.push_back(&DM);
		M.push_back(&LM);
		M.push_back(&TM);

	};
	int CLASSIFY(std::vector<double> feature_vector);

private:
	double rho, eta, lambda, gamma;
	int dimension;	
};

class two_step {
	
public:	

	two_step(double rho, double eta, double nu, double lambda, double gamma, int dimension) : rho(rho), eta(eta), nu(nu), lambda(lambda), gamma(gamma), dimension(dimension),
		dltm (multi_classDLTM(rho, eta, lambda, gamma, dimension)),
		one_cler(one_class(rho, eta, nu, gamma, dimension)),
		binary_cler(binary_class(rho, eta, lambda, gamma, true, dimension)) {};
	int update(int fb);
	int predict(std::vector<double> feature_vector);
	
private:

	double rho, eta, nu, lambda, gamma;
	int dimension;
	one_class one_cler;
	binary_class binary_cler;
	multi_classDLTM dltm;
};
