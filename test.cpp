#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#define TPI	6.283185307179586476925286766559
using namespace std;

class POS {
public:
	double x;
	double y;
	POS( double a, double b ){ x=a; y=b; };	//初期化
	string toStr(int d);		//文字列化
};

string POS::toStr(int d){	//文字列化
	ostringstream oss;
	int X=x*1000000,Y=y*1000000;
	oss << "X" << X << "Y" << Y << "D0" << d << "*" << endl;
	return oss.str();
}

class ENC {
public:
	POS o;		//中心
	double r;	//半径
	double w;	//線幅
	double k;	//変調率
	double pp;	//極対数
	int num;	//座標データ数
	int l;		//本数
	double dw;	//細線幅
	vector<vector<POS>> pos;	//座標リスト
	ENC(double a,double b,double c,double d,double e,double f,double g,double h,double i) : o(a,b)
	{ r=c; w=d; k=e; pp=f; num=g; l=h; dw=i; pos.assign(2*l,vector<POS>()); };	//初期化
	string toStr();		//文字列化
	string toStr2();	//文字列化
};

string ENC::toStr(){	//文字列化
	ostringstream oss;
	for(auto &i : pos){ for(auto j : i){ if(j.x==i.begin()->x&&j.y==i.begin()->y){oss << j.toStr(2);}else{oss << j.toStr(1);} } oss << i.begin()->toStr(1); }
	return oss.str();
}

string ENC::toStr2(){	//文字列化
	ostringstream oss;
	for(int i=0;i<2*l;i+=2)
	{
		for(auto j : pos[i])	//G36とG01
		{
			if( j.x==pos[i].begin()->x && j.y==pos[i].begin()->y)	{ oss << "G36*" << endl; oss << j.toStr(2); oss << "G01*" << endl;} //始点ならG36のD02
			else							{ oss << j.toStr(1); }	//それ以外ならG01のD01
		}
		oss << pos[i].begin()->toStr(1);	//外側の終点（＝始点）をもう一回書く
		i++;	//内側に行く
		oss << pos[i].begin()->toStr(1);	//内側の始点を一回書く
		for(auto itr = pos[i].rbegin(); itr != pos[i].rend(); ++itr)	//内側（逆順）
		{
			oss << itr->toStr(1);	//D01
		};
		oss << pos[i].begin()->toStr(1);	//内側の終点（＝始点）をもう一回書く
		i--;	//外側に行く
		oss << pos[i].begin()->toStr(1);	//外側の終点（＝始点）をもう一回書く	//G01終わり

		for(auto j : pos[i])	//G37
		{
			if( j.x==pos[i].begin()->x && j.y==pos[i].begin()->y)	{ oss << "G37*" << endl; oss << j.toStr(2); } //始点ならD02
			else							{ oss << j.toStr(1); }	//それ以外ならD01
		}
		oss << pos[i].begin()->toStr(1);	//外側の終点（＝始点）をもう一回書く
		i++;	//内側に行く
		oss << pos[i].begin()->toStr(1);	//内側の始点を一回書く
		for(auto itr = pos[i].rbegin(); itr != pos[i].rend(); ++itr)	//内側（逆順）
		{
			oss << itr->toStr(1);	//D01
		};
		oss << pos[i].begin()->toStr(1);	//内側の終点（＝始点）をもう一回書く
		i--;	//外側に行く
		oss << pos[i].begin()->toStr(1);	//外側の終点（＝始点）をもう一回書く	//G37終わり

	}

	return oss.str();
}


int main(int argc, char *argv[]){
	//中心x,中心y,半径,線幅,変調率,極対数,データ数,本数
	if( argc != 10 ){ cout << "argc=" << argc << endl; getchar(); return 0; }	//引数合わない
	ENC enc(stod(argv[1]),stod(argv[2]),stod(argv[3]),stod(argv[4]),stod(argv[5]),stod(argv[6]),stod(argv[7]),stod(argv[8]),stod(argv[9]));	//初期化

	ofstream Fout("test.txt");	//ファイル開く

	for(int j=0;j<2*enc.l;j+=2){
		for(int i=0;i<enc.num;i++){
			double rr=enc.r+0.5*j*enc.w;
			double rp=rr+(+0.25*enc.w-enc.dw)*(1+enc.k*sin(enc.pp*i*TPI/enc.num))+0.5*enc.dw;	//半径
			double rm=rr+(-0.25*enc.w+enc.dw)*(1+enc.k*sin(enc.pp*i*TPI/enc.num))-0.5*enc.dw;	//半径
			double th=i*TPI/enc.num;	//角度
			enc.pos[j  ].emplace_back(enc.o.x+rp*cos(th),enc.o.y+rp*sin(th));	//要素追加
			enc.pos[j+1].emplace_back(enc.o.x+rm*cos(th),enc.o.y+rm*sin(th));	//要素追加
		}
	}
	Fout << enc.toStr2() << endl;	//ファイルに書く

	return 0;
}
