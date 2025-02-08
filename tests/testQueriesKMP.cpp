#include<bits/stdc++.h>
#include<cstdio>
#include<cstring>
#include<string>

using namespace std;

#include "timer.h"
//#include "trace.h"

const int TRIALS = 1000000;
//const int TRIALS = 1000000;
const int FAC = 1000;
#define SEED 346

vector<pair<string,pair<long long, long long>>> names;
vector<vector<double>> times;
int fileC;


/*
void test_validity_queries(){
    cout << "\nTesting validity of queries\n";
    vector<string> v = {"aabaa", "dbdaa", "aadbd"};
    // vector<string> v = {"aab", "aabcd", "bcd"};
    HOG hog(v);
    trace(hog.marked);
    // assert(hog.marked == vector<bool>({0,1,0,1,0,0,1,0,0,1,0,0,1,0,1}));
    Hog hg = hog.make_hog();
    // trace(hg.top(0,3));
    trace(hg.suffix_prefix_length(0,2));
    trace(hg.suffix_prefix_length(2,0));
    trace(hg.suffix_prefix_length(0,1));
    trace(hg.suffix_prefix_length(1,0));
    trace(hg.suffix_prefix_length(2,1));
    trace(hg.suffix_prefix_length(1,2));
    trace(hg.one_to_all(0));
    trace(hg.one_to_all(1));
    trace(hg.one_to_all(2));
    trace(hg.top(0,3));
    trace(hg.top(1,3));
    trace(hg.top(2,3));
    trace(hg.report(0,1));
    trace(hg.report(1,1));
    trace(hg.report(2,1));
    trace(hg.count(0,1));
    trace(hg.count(1,1));
    trace(hg.count(2,1));
    assert(hg.suffix_prefix_length(0,2)==2);
    assert(hg.suffix_prefix_length(2,0)==0);
    assert(hg.suffix_prefix_length(0,1)==0);
    assert(hg.suffix_prefix_length(1,0)==2);
    assert(hg.suffix_prefix_length(2,1)==3);
    assert(hg.suffix_prefix_length(1,2)==2);
    assert((hg.one_to_all(0) == std::vector<int>{5, 0, 2}));
    assert((hg.one_to_all(1) == std::vector<int>{2, 5, 2}));
    assert((hg.one_to_all(2) == std::vector<int>{0, 3, 5}));
    trace(hg.top(0,3));
    trace(hg.top(1,3));
    trace(hg.top(2,3));
    assert((hg.report(0,1) == std::vector<int>{0,2}));
    assert((hg.report(1,1) == std::vector<int>{1, 0, 2}));
    assert((hg.report(2,1) == std::vector<int>{2, 1}));
    assert((hg.count(0,1) == 2));
    assert((hg.count(1,1) == 3));
    assert((hg.count(2,1) == 2));
    cout<<"All tests passed\n";
}
*/

pair<double, double> get_mean_and_sd(vector<double> &a) {
    sort(a.begin(), a.end());
    double sum = 0, sq_sum = 0, cnt=0;
    for(int i=a.size()/10;i<(int)(9*a.size()/10);i++) {
        sum += a[i];
        sq_sum += a[i]*a[i];
        cnt++;
    }
    double avg = sum/cnt, sd = sqrt(sq_sum/cnt - avg*avg);
    return {avg, sd};
}


/*void stress_test_with(const vector<string>& v, int seed) {
    HOG hog;
    hog.add_strings(v);
    timer t1;
    hog.construct();
    std::cout << t1.end() << std::endl;

    timer t2;
    Hog hg = hog.make_hog();
    std::cout << t2.end() << "\n";
    std::cout << "Construction Finished" << std::endl;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS);
    for(int i = 0;i<TRIALS;i++){
        int a = rand()%hg.sorted_order_conversion.size();
        int b = rand()%hg.sorted_order_conversion.size();
        if(a==b)i--;
        else{
            queries[i] = {a,b};
        }
    }
    timer q_time;
    for(int i = 0;i<TRIALS;i++){
        hg.suffix_prefix_length(queries[i].first, queries[i].second);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for one-to-one " << TRIALS << "queries is " << tot_time << "\n";
}


void stress_test_with_all(const vector<string>& v, int seed) {
    HOG hog;
    hog.add_strings(v);
    timer t1;
    hog.construct();
    std::cout << t1.end() << std::endl;

    timer t2;
    Hog hg = hog.make_hog();
    std::cout << t2.end() << "\n";
    std::cout << "Construction Finished" << std::endl;
    srand(seed);
    std::vector<int>queries(TRIALS / FAC);
    for(int i = 0;i<TRIALS/FAC;i++){
        queries[i] = rand()%hg.sorted_order_conversion.size();
    }
    timer q_time;
    for(int i = 0;i<TRIALS/FAC;i++){
        hg.one_to_all(queries[i]);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for one-to-all " << TRIALS/FAC << "queries is " << tot_time << "\n";
}
void stress_test_with_top(const vector<string>& v, int seed) {
    HOG hog;
    hog.add_strings(v);
    timer t1;
    hog.construct();
    std::cout << t1.end() << std::endl;

    timer t2;
    Hog hg = hog.make_hog();
    std::cout << t2.end() << "\n";
    std::cout << "Construction Finished" << std::endl;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS/FAC);
    for(int i = 0;i<TRIALS/FAC;i++){
        int a = rand()%hg.sorted_order_conversion.size();
        int b = rand()%hg.sorted_order_conversion.size() + 1;
        queries[i] = {a,b};
    }
    timer q_time;
    for(int i = 0;i<TRIALS/FAC;i++){
        hg.top(queries[i].first, queries[i].second);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for top " << TRIALS/FAC << "queries is " << tot_time << "\n";
}
void stress_test_with_report(const vector<string>& v, int seed) {
    HOG hog;
    hog.add_strings(v);
    timer t1;
    hog.construct();
    std::cout << t1.end() << std::endl;

    timer t2;
    Hog hg = hog.make_hog();
    std::cout << t2.end() << "\n";
    std::cout << "Construction Finished" << std::endl;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS/FAC);
    for(int i = 0;i<TRIALS/FAC;i++){
        int a = rand()%hg.sorted_order_conversion.size();
        int b = rand()%10 + 1;
        queries[i] = {a,b};
    }
    timer q_time;
    for(int i = 0;i<TRIALS/FAC;i++){
        hg.report(queries[i].first, queries[i].second);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for report " << TRIALS/FAC << "queries is " << tot_time << "\n";
}
void stress_test_with_count(const vector<string>& v, int seed) {
    HOG hog;
    hog.add_strings(v);
    timer t1;
    hog.construct();
    std::cout << t1.end() << std::endl;

    timer t2;
    Hog hg = hog.make_hog();
    std::cout << t2.end() << "\n";
    std::cout << "Construction Finished" << std::endl;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS/FAC);
    for(int i = 0;i<TRIALS/FAC;i++){
        int a = rand()%hg.sorted_order_conversion.size();
        int b = rand()%10 + 1;
        queries[i] = {a,b};
    }
    timer q_time;
    for(int i = 0;i<TRIALS/FAC;i++){
        hg.count(queries[i].first, queries[i].second);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for count " << TRIALS/FAC << "queries is " << tot_time << "\n";
}*/


// Modified from GeeksForGeeks
void KMPinit(vector<vector<int> >& lps, const vector<string>& v){
	lps.resize(v.size());
	for(int j=0;j<v.size();j++){
		lps[j].resize(v[j].length());

	       int len = 0;     // Length of the previous longest prefix suffix
	       lps[j][0] = 0;   // lps[0] is always 0

		int i = 1;     // loop calculates lps[i] for i = 1 to M-1
    		while (i < v[j].length()) {
        		if (v[j][i] == v[j][len]) {
 				len++;
            			lps[j][i] = len;
            			i++;
        		}
        		else // (v[j][i] != v[j][len])
        		{
            			if (len != 0) {
                			len = lps[j][len - 1];
            			}
            			else // if (len == 0)
            			{
                			lps[j][i] = 0;
                			i++;
            			}
        		}
    		}
	}
}



// Modified from GeeksForGeeks
int KMPSearch(const vector<vector<int> >& lps, const vector<string>& v, int pat, int txt)
{
	int M = v[pat].length();
	int N = v[txt].length();

	int result=0;
    
    	int i = 0; // index for txt
    	int j = 0; // index for pat
    	
	while ((N - i) >= (M - j)) {
        	if (v[pat][j] == v[txt][i]) {
            		j++;
            		i++;
        	}

        	if (j == M) {
            		result=M;
            		j = lps[pat][j - 1];
        	}

        	// Mismatch after j matches
        	else if (i < N && v[pat][j] != v[txt][i]) {

		    if(result<j-1)
		    	result=j-1;		

            // Do not match lps[0..lps[pat][j-1]] characters,
            // they will match anyway
	            if (j != 0)
	                j = lps[pat][j - 1];
	            else
	                i = i + 1;
        	}
    }
    return result;
}



void stress_test_with(const vector<vector<int> >& lps, const vector<string>& v, int seed) {
    int result;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS);
    for(int i = 0;i<TRIALS;i++){
        int a = rand()%v.size();
        int b = rand()%v.size();
        if(a==b)i--;
        else{
            queries[i] = {a,b};
        }
    }
    timer q_time;
    for(int i = 0;i<TRIALS;i++){
 	result=KMPSearch(lps,v,queries[i].second,queries[i].first);
//        hg.suffix_prefix_length(queries[i].first, queries[i].second);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for one-to-one " << TRIALS << "queries is " << tot_time << "\n";

	times[fileC].push_back(tot_time*1000/TRIALS);
}


void stress_test_with_all(const vector<vector<int> >& lps, const vector<string>& v, int seed) {
    vector<int> result(v.size());
    srand(seed);
    std::vector<int>queries(TRIALS / FAC);
    for(int i = 0;i<TRIALS/FAC;i++){
        queries[i] = rand()%v.size();
    }
    timer q_time;
    for(int i = 0;i<TRIALS/FAC;i++){
	    for(int j = 0;j<v.size();j++)
	 	result[j]=KMPSearch(lps,v,j,queries[i]);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for one-to-all " << TRIALS/FAC << "queries is " << tot_time << "\n";

	times[fileC].push_back(tot_time*FAC*1000/TRIALS);

}


void stress_test_with_top(const vector<vector<int> >& lps, const vector<string>& v, int seed) {
    priority_queue <int, vector<int>, greater<int> > result; 
    srand(seed);
    vector<pair<int,int>>queries(TRIALS/FAC);
    for(int i = 0;i<TRIALS/FAC;i++){
        int a = rand()%v.size();
        int b = rand()%v.size() + 1;
        queries[i] = {a,b};
    }
    timer q_time;
    for(int i = 0;i<TRIALS/FAC;i++){
	    for(int j = 0;j<v.size();j++){
	 	result.push(KMPSearch(lps,v,j,queries[i].first));
	    	if(j>queries[i].second) result.pop();
	    }
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for top " << TRIALS/FAC << "queries is " << tot_time << "\n";

	times[fileC].push_back(tot_time*FAC*1000/TRIALS);
}


void stress_test_with_report(const vector<vector<int> >& lps, const vector<string>& v, int seed) {
    vector<int> result;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS/FAC);
    for(int i = 0;i<TRIALS/FAC;i++){
        int a = rand()%v.size();
        int b = rand()%10 + 1;
        queries[i] = {a,b};
    }
    timer q_time;
    for(int i = 0;i<TRIALS/FAC;i++){
    	    result.clear();
	    for(int j = 0;j<v.size();j++){
	 	if(KMPSearch(lps,v,j,queries[i].first)>queries[i].second)
	 		result.push_back(j);
	    }
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for report " << TRIALS/FAC << "queries is " << tot_time << "\n";

	times[fileC].push_back(tot_time*FAC*1000/TRIALS);
}

void stress_test_with_count(const vector<vector<int> >& lps, const vector<string>& v, int seed) {
    int result;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS/FAC);
    for(int i = 0;i<TRIALS/FAC;i++){
        int a = rand()%v.size();
        int b = rand()%10 + 1;
        queries[i] = {a,b};
    }
    timer q_time;
    for(int i = 0;i<TRIALS/FAC;i++){
    	    result=0;
	    for(int j = 0;j<v.size();j++){
	 	if(KMPSearch(lps,v,j,queries[i].first)>queries[i].second)
	 		result++;
	    }
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for count " << TRIALS/FAC << "queries is " << tot_time << "\n";

	times[fileC].push_back(tot_time*FAC*1000/TRIALS);
}


void real_data_test() {
    fileC=0;
    
    vector<vector<int> > lps;

    string data_path = "../data/";
//     vector<string> filenames = {"clementina", "sinensis", "trifoliata", "elegans"};
    // vector<string> filenames = {"trifoliata"};
    vector<string> filenames = { "A_hydrophila_HiSeq",
                                "B_cereus_HiSeq",
                                "B_cereus_MiSeq",
                                "B_fragilis_HiSeq" ,
                                "M_abscessus_HiSeq" ,
                                "M_abscessus_MiSeq",
                                "R_sphaeroides_HiSeq" ,
                                "R_sphaeroides_MiSeq" ,
                                "S_aureus_HiSeq",
                                "V_cholerae_HiSeq" ,
                                "V_cholerae_MiSeq",
                                "X_axonopodis_HiSeq"}; 


 	names.resize(filenames.size());
  	times.resize(filenames.size());


    for(string fname:filenames) {
        cout<<'\n'<<fname<<":\n";
        fstream fin;
        fin.open(data_path+fname, ios::in);
        if(!fin) {
            cout<<"couldn't open file: "<<fname<<endl;
            continue;
        }
 
  	
        long long n, total_length = 0;
        fin>>n;
        vector<string> v(n);
        for(int i=0;i<n;i++) {
            fin>>v[i];
            total_length += v[i].length();
        }

        cout<<"Number of strings = "<<v.size()<<'\n'<<"Sum of lengths = "<<total_length<<'\n';
	names[fileC] = {fname,{v.size(),total_length}};
        
    	timer t1;
        KMPinit(lps,v);
    	std::cout << t1.end() << std::endl;
  	       
        stress_test_with(lps,v, SEED);
        stress_test_with_all(lps,v, SEED);
        stress_test_with_top(lps,v,SEED);
        stress_test_with_count(lps,v, SEED);
        stress_test_with_report(lps,v, SEED);

	fileC++;
    }
}

int main(){
 //   test_validity_queries();
    real_data_test();
 
double HOG[12][5] ={ 0.010 , 95.375 , 34.238 , 8.434 , 10.783,
 			0.009 , 71.926 , 30.076 , 4.433 , 8.397,
			0.003 , 8.654 , 4.070 , 0.698 , 0.922,
		        0.008 , 74.648 , 30.248 , 4.583 , 6.302,
			0.002 , 28.556 , 11.384 , 1.627 , 2.376,
			0.002 , 7.945 , 4.026 , 0.656 , 0.883,
			0.003 , 51.254 , 19.836 , 2.811 , 4.310,
			0.002 , 5.954 , 2.925 , 0.516 , 0.722,
			0.005 , 32.004 , 13.138 , 1.832 , 2.729,
			0.002 , 15.161 , 6.823 , 1.085 , 1.451,
			0.002 , 5.826 , 2.967 , 0.510 , 0.670,
			0.009 , 76.691 , 30.474 , 5.002 , 7.976};
 
    
cout<<"Table1 "<<'\n';    
for(int i = 0;i<fileC;i++)
	cout<<names[i].first<<" & "<<names[i].second.first<<" & "<<names[i].second.second<<" & "<< 
	           times[i][0]<<" & "<<times[i][1]<<" & "<<times[i][2]<<" & "<<times[i][3]<<" & "<<times[i][4]<<'\n';
	          
	           
cout<<'\n'<<"Table2 "<<'\n';    
    
for(int i = 0;i<fileC;i++)
	cout<<" &  &  & "<< 
	           times[i][0]<<" & "<<times[i][1]<<" & "<<times[i][2]<<" & "<<times[i][3]<<" & "<<times[i][4]<<'\n';

    
    

for(int i = 0;i<fileC;i++){
	cout<<" &  &  & "<< 
	           times[i][0]<<" & "<<times[i][1]<<" & "<<times[i][2]<<" & "<<times[i][3]<<" & "<<times[i][4]<<"\\\\"<<'\n';
	cout<<" &  &  & $"<< 
	           times[i][0]/HOG[i][0]<<"\\times$ & $"<< 
	           times[i][1]/HOG[i][1]<<"\\times$ & $"<< 
	           times[i][2]/HOG[i][2]<<"\\times$ & $"<< 
	           times[i][3]/HOG[i][3]<<"\\times$ & $"<< 
	           times[i][4]/HOG[i][4]<<"\\times$ \\\\" <<'\n';	           
	         }

cout<<'\n'<<"Table4 "<<'\n';    


for(int i = 0;i<fileC;i++){
	cout<< names[i].first;
	printf("& %lld & %lld & HOG & %.3f & %.3f & %.3f & %.3f & %.3f \\\\ \n", 
	      names[i].second.first, names[i].second.second,HOG[i][0],HOG[i][1],HOG[i][2],HOG[i][3],HOG[i][4]);
	printf(" & & & KMP & %.5f & %.3f & %.3f & %.3f & %.3f \\\\ \n", 
	           times[i][0], times[i][1], times[i][2] , times[i][3], times[i][4]);
	printf(" & & & Rel & %.2f$\\times$ & %.2f$\\times$ & %.2f$\\times$ & %.2f$\\times$ & %.2f$\\times$ \\\\ \\hline \n", 
	           times[i][0]/HOG[i][0], times[i][1]/HOG[i][1], times[i][2]/HOG[i][2], 
	           times[i][3]/HOG[i][3], times[i][4]/HOG[i][4]);	           
	         }


}

