#include <vector>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <set>
#include <iostream>

using namespace std;
/*******************************************-----Data-----**********************************************/
/* nhap so luong thanh pho,tao ma tran thanh pho                                                       */                                          
/* tao ma tran chi phi -khoang cach                                                                    */
/* tao ma tran heuristic  1/chi phi tung thanh pho                                                     */
/* tao nong do vet mui bang 1 het                                                                      */
/* tao ham xu ly tính chi phi                                                                          */
/*******************************************-----Data-----**********************************************/
class Data {
public:
	vector< pair<double,double> > cityMatrix;// toa do cua cua cac thanh pho
	vector< vector<double> > cost;// ma tran khoang cach cung la chi phi di chuyen
	vector< vector<double> > heuristic;// luu ma tran heuristic
	vector< vector<double> > T;// nong do vet mui-duoc cap nhat sau moi con kien
	int N;// so thanh pho
	double arr[10000][10000];// luu ma tran khoang cach giua cac thanh pho
	double X[1000000],Y[1000000];// X[]: luu hoanh do;Y[]: luu tung do 

	Data() {	
	
		scanf("%d",&N);							// nhap so thanh pho
		cityMatrix.push_back(make_pair(0,0));	
		vector< double> L,M,U; 					//tao 3 vector de xuly khoi tao cho vector hai chieu
		cost.push_back(L);			
		T.push_back(M);
		heuristic.push_back(U);

		for(int i=0;i<N;i++) {
			double x,y;
			scanf("%lf%lf",&x,&y);				//nhap toa do tung thanh pho
			cityMatrix.push_back(make_pair(x,y));
			X[i]=x;
			Y[i]=y;
			
		}
//tao ma tran khoang cach giua cac thanh pho//////////////////////////////////////		
		for (int i=0;i<N;i++){
			for (int j=0;j<N;j++){
				arr[i][j]=sqrt((X[i]-X[j])*(X[i]-X[j])+(Y[i]-Y[j])*(Y[i]-Y[j]));
			}
		}
///////////////////////////////////////////////////////////////////////////////////

//Tao ma tran heuristic,ma tran khoang cach,nong do mui
		for(int i=0;i<N;i++) {
			vector<double> V(N+1);
			vector<double> t(N+1);
			vector<double> l(N+1);
			for(int j = 1; j<=N ; j++) {
				t[j] = 1.0;
				V[j]=arr[i][j-1];
				if(arr[i][j-1] != 0) {
					l[j] = 1/ arr[i][j-1];
				}
			}
			cost.push_back(V);// tao ma tran khoang cach giong ma tran 
							//arr[][] de xu ly khoang cach cua kien sau nay
			T.push_back(t);/// khoi tao bang 1
			heuristic.push_back(l);//tao ma tran heuristic
		}

	}


//chi phi hanh trinh////////////////////////////////////////////////////
	double tourCost(vector<int> C) {
		int l = C.size();
		double tourCost = 0.0;
		l = l-1;
		for (int i=0;i <l; i++) {
			tourCost += cost[C[i]][C[i+1]];
		}
		tourCost += cost[C[l]][C[0]];
		return tourCost;
	}
/////////////////////////////////////////////////////////////////////////
//in ra ket qua luu tai vector C////////////////////////////////////////
	void print(vector<int> C) {
		for(int i=0;i<C.size();i++)
			printf("%d ",C[i]);
		printf("\n");
	}
////////////////////////////////////////////////////////////////////////
};
/*****************************************************************************************/

Data d;											

/*****************************************************************************************/
class Ant {
public:
	vector<int> trail;		//luu lop mui(chu trinh di chuyen)
	set<int> available;     //luu qua trinh di chuyen(cap nhat lai cua tung con kien)
	double alpha;			// he so dieu chinh mui						
	double beta;			// he so dieu chinh hueristic
	double evaporation;		//toc do bay hoi mui						
	Ant(double a,double b,double e) {// khoi tao kien 
		alpha = a;
		beta = b;
		evaporation=e;
		trail.push_back(1);						// always start from the nest (1)						
		for(int i=2;i<=d.N;i++) {				// d.N so thanh pho co tu lop Data:d
			available.insert(i);				
		}
	}
	void reset() {   // ham khoi tao cua con kien moi
		vector<int> L;
		L.push_back(1);
		trail = L;							
		for(int i=2;i<=d.N;i++) {
			available.insert(i);
		}
	}
	void deposit() {                  // cap nhat lai mui
		double tourCost = d.tourCost(trail);
		int Q=100;                   // he so dieu chinh mu
		double depositAmount = Q / tourCost;// denta (i,j)
		int l = trail.size();
		l = l-1;
		for (int i=0;i <l; i++) {
//			d.T[trail[i]][trail[i+1]] += depositAmount;
			d.T[trail[i]][trail[i+1]] = d.T[trail[i]][trail[i+1]]*(1-evaporation)+depositAmount;
		}
//		d.T[trail[l]][trail[0]] += depositAmount;
		d.T[trail[l]][trail[0]] = d.T[trail[l]][trail[0]]*(1-evaporation)+depositAmount;
	}

	vector<int> stop() {// ket thuc va tra ve lop nong do mui(tra ve chu trinh di chuyen)
		deposit();
		vector<int> temp = trail;
		reset();
		return temp;
	}

	void step() {
		int currentCity = trail[trail.size()-1];
		double norm = probabilityNorm(currentCity);    
		double p,gp;
		bool moved = false;
		double highestProb = 0;// xac suat cua thanh pho co nong do mui cao nhat hien tai
		double cityHighest = 0;// vi tri cua thanh pho co highestProb
		for(set<int>::iterator i=available.begin(); i != available.end() ; i++) {
			p = moveProbability(currentCity,*i,norm);
			if (p > highestProb) {
				cityHighest = *i;
				highestProb = p;
			}
			gp = getRand();
			if (gp <= p) { // chuyen huong di chuyen
				moved = true;
				trail.push_back(*i);
				available.erase(i);   
				break;
			}
		}
		if(!moved) { 
			trail.push_back(cityHighest);
			available.erase(cityHighest);
		}
	}
//random chon thanh pho ///////////////////////////////////////////////////////////////
	double getRand() {
			double p = (rand() / (RAND_MAX + 1.0));
	    return p;
	}
////////////////////////////////////////////////////////////////////////////////////////////////

//xac suat di chuyen///////////////////////////////////////////////////////////////////////////
	double moveProbability(int i,int j,double norm) {// xac xuat di chuyen
		double p = (pow(d.T[i][j],alpha))*(pow(d.heuristic[i][j],beta));
		p /= norm;
		return p;
	}
///////////////////////////////////////////////////////////////////////////////////////////////
// hanh trinh kien tinh mau so////////////////////////////////////////////////////////////////////
	double probabilityNorm(int currentCity) {
		int size = available.size();
		double norm = 0.0;
		for(set<int>::iterator i=available.begin(); i != available.end() ; i++) {
			norm += (pow(d.T[currentCity][*i],alpha))*(pow(d.heuristic[currentCity][*i],beta));
		}
		return norm;
	}

};
/*****************************************************************************************/

/**************************************-----ACO------***************************************************/
class ACO {
public:
	int N;					// so thanh pho
	int M;					// so con kien
	vector<Ant> ant;	
	double evaporation;			// he so bay hoi mui
	double alpha;				// he so anh huong mui
	double beta;				// he so anh huong heuristic

	ACO(double a,double b,double e) {
		alpha = a;
		beta = b;
		evaporation = e;
		N = d.N;
		M = 500;															
		for(int i=0; i<M; i++) {
			Ant a(alpha,beta,e);
			ant.push_back(a);
		}
	}

	void run() {
		vector<int> PATH;  // noi luu chu trinh(luu ket qua bai toan)
		double minTour,tourC;
		for(int n=0;n < 30; n++) {    // 30 la so vong lap (do desgin tu chon)
			for(int p=0; p<(N-1); p++){
				for(int i=0;i<M;i++) {
					ant[i].step();
				}
			}
			for(int i=0;i<M;i++) {
				vector<int> p = ant[i].stop();
				if(!PATH.size()) {
					PATH = p;
					minTour = d.tourCost(p); 
					continue;
				}
				tourC = d.tourCost(p);
				if(tourC < minTour) {
					minTour = tourC;
					PATH = p;// cap nhat neu chi phi nho hon
				}
			}
///////cap nhat lai nong do mui/////////////////////////
//			for(int i=1;i<=N;i++) {
//				for(int j=1;j<=N;j++) {
//					d.T[i][j] *= (1-evaporation);
//				}
//			}
///////////////in ra chu trinh///////////////////////////////////////
		}
		printf("%lf\n",minTour);
		d.print(PATH);
	}
};
/*****************************************************************************************/

/***********************************-----MAIN-----***********************************************/
int main(void) {
	srand((int)time(0));
	double alpha = 1;						// pheromone importance
	double beta = 2;						// heuristic importance
	double evaporation = 0.2;					// evaporation rate: toc do bay hoi mui
	ACO colony(alpha,beta,evaporation);
	colony.run();
	return 0;
}
/*****************************************************************************************/
/*
23
1 22  
6 2   
28 12
16 24
23 4
28 27
2 28
20 23
30 21
12 18
15 20
16 18
29 30
30 23
34 23
15 15
17 17
20 20
5 10
20 21
3 4
10 15
10 10
*/

