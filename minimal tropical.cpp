#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include <cstring>
#include <sstream>
#include <map>
#include <conio.h>
#include <ilcplex\ilocplex.h>

using namespace std;

ifstream fin;
ifstream qin;
ofstream fout("answers taghribi.txt");
double limit=300;
clock_t start;
double gettime(){
	return ((double)((clock() - start)))/CLOCKS_PER_SEC;
}

bool timelimit=false;

class graph{
    public:
    int v;
	set<int> rous;
	map<int,set<int> > colors;
    map<int,set<int> >adj;
	map<int,int> rang;
    set<pair<int,int> >degrees;
    graph(int n,map<int,set<int> >a){
      v=n;
      adj=a;
      degrees.clear();
	  rang.clear();
	  rous.clear();
      for(int i=0;i<v;i++){
        degrees.insert(make_pair(adj[i].size(),i));
		 rous.insert(i);
	  }
    }
    graph(){
		v=0;
		adj.clear();
		degrees.clear();
		rous.clear();
	};
	int numedges(){
		int ted=0;
		map<int,set<int> >::iterator it=adj.begin();
		while(it!=adj.end()){
			ted+=it->second.size();
			it++;
		}
		return ted/2;
	}
    void print(){
      cout<<v<<endl;
      for(map<int,set<int> >::iterator i=adj.begin();i!=adj.end();i++){
        cout<<i->first<<":  ";
        for(set<int>::iterator  j=i->second.begin();j!=i->second.end();j++)
          cout<<*j<<' ';

      cout<<endl;
      }
    }
    bool is_triangle(int a,int b,int c){
      bool b1=((adj[a].find(b)!=adj[a].end())&(adj[a].find(c)!=adj[a].end()));
      bool b2=(adj[b].find(c)!=adj[b].end());
      return (b1&b2);
    }
    void read(){
      string s;
      char c;
      while(1){
        fin>>c;
        if(c=='c'){
          getline(fin,s);
          //cerr<<s<<endl;
        //  getch();
        }
        if(c=='p'){
          string f;
          fin>>f;
          long e;
          fin>>v>>e;
         // cerr<<v<<' '<<e<<endl;
         // getch();
          degrees.clear();
          for( int i=0;i<adj.size();i++)
            adj[i].clear();
          for(int i=0;i<e;i++){
            fin>>c;
            int p,q;
            fin>>p>>q;
            p--;
            q--;
            adj[p].insert(q);
            adj[q].insert(p);
          }
          for(int i=0;i<v;i++){
			rous.insert(i);
            degrees.insert(make_pair(adj[i].size(),i));
		  }
          break;
        }
      }
	  int cc;
	  for(int i=0;i<v;i++){
		 qin>>cc;
		 colors[cc].insert(i);
		 rang[i]=cc;
	  }
	return;
    }
    void read2(){
      long e;
      cin>>v>>e;
	  rous.clear();
      degrees.clear();
      for(int i=0;i<adj.size();i++)
        adj[i].clear();
      for(int i=0;i<e;i++){

        int p,q;
        cin>>p>>q;
        adj[p].insert(q);
        adj[q].insert(p);
      }
	  for(int i=0;i<v;i++)
		  rous.insert(i);
      return;
    }
};

graph operator-(graph g,long c){
    graph ans=g;
	if(g.rous.find(c)==g.rous.end())
		return ans;
    pair<long,long> f=make_pair(ans.adj[c].size(),c);
    for(set<int>::iterator  i=ans.adj[c].begin();i!=ans.adj[c].end();i++){
        long u=*i;
        long degu=ans.adj[u].size();
        pair<long,long> p=make_pair(degu,u);
        ans.degrees.erase(p);
        if(degu>1)
          ans.degrees.insert(make_pair(degu-1,u));
        ans.adj[u].erase(c);
    }
    ans.adj.erase(c);
	ans.rang.erase(c);
	ans.rous.erase(c);
    ans.v--;
    ans.degrees.erase(f);
    return ans;
}


class triangle{
    public:
      int a,b,c;
      triangle(int d,int e,int f){
        a=d;
        b=e;
        c=f;
      }
      void print(){
        cout<<a<<' '<<b<<' '<<c<<endl;
      }
};
graph operator-(graph g,triangle c){
    graph ans=g;
    ans=ans-c.a;
    ans=ans-c.b;
    ans=ans-c.c;
    return ans;
}

graph operator-(graph g,pair<int,int> x){
    graph ans=g;
   int a=x.first;
   int b=x.second;
   pair<int,int> l=make_pair(ans.adj[a].size(),a);
   ans.adj[a].erase(b);
   ans.degrees.erase(l);
   if(ans.adj[a].size()==0){
      ans.adj.erase(a);
      ans.v--;
   }
   else
      ans.degrees.insert(make_pair(l.first-1,a));
    l=make_pair(ans.adj[b].size(),b);
   ans.adj[b].erase(a);
   ans.degrees.erase(l);
   if(ans.adj[b].size()==0){
      ans.adj.erase(b);
      ans.v--;
   }
   else
      ans.degrees.insert(make_pair(l.first-1,b));
    return ans;
}
graph adjdelete(graph g,set<int> comp){
  set<int>r;
  r.clear();
  for(set<int>::iterator it=comp.begin();it!=comp.end();it++)
    for(set<int>::iterator it2=g.adj[*it].begin();it2!=g.adj[*it].end();it2++)
      r.insert(*it2);
  comp.insert(r.begin(),r.end());
  for(set<int>::iterator it=comp.begin();it!=comp.end();it++)
    for(set<int>::iterator it2=comp.begin();it2!=comp.end();it2++)
        g.adj[*it].erase(*it2);

  return g;
}
int common(set<int> a,set<int> b){
  set<int>::iterator it=a.begin();
  int ted=0;
  while(it!=a.end()){
    ted+=(b.find(*it)!=b.end());
    it++;
  }
  return ted;
}
bool equall(int a[],int b[],int siz){
  for(int i=0;i<siz;i++)
      if(a[i]!=b[i])
        return false;
    return true;
}
set<int> bfs(graph g,int a){
  map<int,bool> checked;
  checked.clear();
  checked[a]=true;
  vector<int> fehrest;
  fehrest.clear();
  fehrest.push_back(a);
  for( int i=0;i<fehrest.size();i++){
    set<int>::iterator it=g.adj[fehrest[i]].begin();
    while(it!=g.adj[fehrest[i]].end()){
      if(!checked[*it]){
        fehrest.push_back(*it);
        checked[*it]=true;
      }
      it++;
    }
  }
  set<int> r;
  r.clear();
  r.insert(fehrest.begin(),fehrest.end());
  return r;
}

set<int> bfs(graph g,set<int> a){
  set<int> fehrest;
  fehrest.clear();
  for(set<int>::iterator it=a.begin();it!=a.end();it++){
    set<int> b=bfs(g,*it);
    fehrest.insert(b.begin(),b.end());
  }
  return fehrest;
}

set<set<int> > components(graph g){
	map<int,set<int> >::iterator it=g.adj.begin();
	set<set<int> > ans;
	ans.clear();
	while(it!=g.adj.end()){
		int u=it->first;
		ans.insert(bfs(g,u));
		it++;
	}
	return ans;
}
vector<int> minimal_sep(graph g,set<int> u,set<int> v){
  set<int> q;
  q.clear();
  for(set<int>::iterator it=u.begin();it!=u.end();it++)
    for(set<int>::iterator it2=g.adj[*it].begin();it2!=g.adj[*it].end();it2++)
      if(u.find(*it2)==u.end())
        q.insert(*it2);
  //cerr<<*q.begin()<<endl;
  g=adjdelete(g,u);
  set<int> f=bfs(g,v);
  vector<int> ans;
  ans.clear();
  for(set<int>::iterator i=f.begin();i!=f.end();i++)
    if(q.find(*i)!=q.end())
      ans.push_back(*i);
  return ans;

}
void wr(set<int> a){
	set<int>::iterator it=a.begin();
	while(it!=a.end()){
		cerr<<*it+1<<' ';
		it++;
	}
	cerr<<endl;
	return;
}
class typ{
	public:
		IloModel mod;
		IloNumVarArray x;
		IloEnv en;
		typ(IloEnv env,graph g){
			en=env;
			mod=IloModel(env);
			x=IloNumVarArray(env,g.v,0,1,ILOFLOAT);
			mod.add(x);
		}
		typ(){}
};
int tim=0;
double getval(typ m){
	IloCplex solver(m.mod);
	solver.solve();
	if(solver.getStatus()==IloAlgorithm::Infeasible){
		solver.end();
		return 1000;
	}
	double u=solver.getObjValue();
	solver.end();
	return u;
}
set<int> bfs(graph g,int a,IloNumArray sval){
  map<int,bool> checked;
  checked.clear();
  checked[a]=true;
  vector<int> fehrest;
  fehrest.clear();
  fehrest.push_back(a);
  if(sval[a]>0.99)
	for( int i=0;i<fehrest.size();i++){
		set<int>::iterator it=g.adj[fehrest[i]].begin();
			while(it!=g.adj[fehrest[i]].end()){
				 if((!checked[*it])&&(sval[*it]>0.99)){
					fehrest.push_back(*it);
					checked[*it]=true;
				}
				it++;
			}
  }
  set<int> r;
  r.clear();
  r.insert(fehrest.begin(),fehrest.end());
  return r;
}
double best;
vector<int> A(graph g,set<int> u){
	vector<int> q;
	q.clear();
	for(set<int>::iterator it=u.begin();it!=u.end();it++)
		for(set<int>::iterator it2=g.adj[*it].begin();it2!=g.adj[*it].end();it2++)
			if(u.find(*it2)==u.end())
				q.push_back(*it2);
	return q;
}
bool okcon(vector<int> u,IloNumArray sval){	
	double jam=0;
	for(int i=0;i<u.size();i++)
		jam+=sval[u[i]];		
	return (jam>=1);
}
int bar=0;
typ hambandsaz(typ model,graph g,double javab){
	if(gettime()>limit){
		timelimit=true;
		return model;
	}
	IloEnv env=model.en;
	double mini=0;
	int cc=0;
	double xz=getval(model);
	if(xz>best)
		return model;
	while(1){
		IloCplex solver(model.mod);
		solver.setOut(env.getNullStream());
		solver.solve();
		bar++;
	//	solver.exportModel(("mymodel"+to_string(bar)+".lp").c_str());
//		cerr<<xz<<endl;
		if(xz>best)
			return model;
		//fout<<"after "<<bar<<" constraints added,obj value is :"<<xz<<endl;
		//if(javab==xz)
		//	return model;
		mini=xz;
		IloNumArray sval(env);
		solver.getValues(sval,model.x);
		//md=model;
		//jav=sval;
		solver.end();
	  //cerr<<sval<<endl;
	  //getch();
		int fir=-1;
		for(int i=0;i<sval.getSize();i++)
		  if((sval[i]>0.99)){
			  fir=i;
			  break;
		  }
		if(fir==-1)
			return model;
		set<int> w=bfs(g,fir,sval);
		set<int> shenakhte=w;
		vector<set<int> >moal;
		moal.clear();
		vector<int> conon;
		conon.clear();
		conon.push_back(fir);
		moal.push_back(w);
//		cerr<<fir<<endl;
//		cerr<<sval<<endl;
//		wr(w);
//		getch();
//	moal.push_back(w);
 // wr(bfs(t,0));
		bool ok=true;
		for(int i=fir+1;i<sval.getSize();i++)
			if((sval[i]>0.99)&&(shenakhte.find(i)==shenakhte.end())){
				ok=false;
				set<int> y=bfs(g,i,sval);
				moal.push_back(y);
				shenakhte.insert(y.begin(),y.end());
				conon.push_back(i);
			//	break;
			}
	 // t.print();
	 // cerr<<p.size()<<endl;
		if(!ok){
			cerr<<moal.size()<<endl;
			bool added=false;
			vector<vector<int> >comps;
			comps.clear();
			for(int i=0;i<moal.size();i++)
				comps.push_back(A(g,moal[i]));
			for(int l=0;l<moal.size();l++)
				for(int j=l+1;j<moal.size();j++){	
					vector<int> z=comps[l];
					
				//	vector<int> z=minimal_sep(g,moal[l],moal[j]);
					if(!okcon(z,sval)){
					//	cerr<<"mabda: ";
					//	wr(moal[l]);
					//	cerr<<"maghsad: ";
					//	wr(moal[j]);
						//cerr<<"bayad begzare az: ";
						added=true;
						IloExpr exp(env);
						for( int i=0;i<z.size();i++){
						//	 cerr<<z[i]+1<<' ';
							exp+=model.x[z[i]];
						}
						model.mod.add(IloConstraint(exp>=model.x[conon[l]]+model.x[conon[j]]-1));
						//break;
						//cerr<<endl<<endl;
					//	getch();
					}
					vector<int> z2=comps[j];
					if(!okcon(z2,sval)){
						IloExpr exp2(env);
                        added=true;					
						for(int i=0;i<z2.size();i++)
							exp2+=model.x[z2[i]];
		//  int l=*w.begin();
		 // int k=*y.begin();
					
						model.mod.add(IloConstraint(exp2>=model.x[conon[l]]+model.x[conon[j]]-1));
					}
			}
			if(added==false)
				return model;
			else
				return hambandsaz(model,g,xz);
		//  getch();
		//  cerr<<sval<<endl;
		}
		else{	
		//	cerr<<sval<<endl;
			//t.print();
			return model;
			break;
		}
  }
  return model;
}

typ rec(typ asghar,graph g){
	if(gettime()>limit){
		timelimit=true;
		return asghar;
	}
	tim++;
	typ voroodi=hambandsaz(asghar,g,-1);
	bool ok=true;
	IloCplex solver(voroodi.mod);
	//solver.exportModel(("mymodel"+to_string(tim)+".lp").c_str());
	solver.solve();
	double x=getval(voroodi);
	if(x>best)
		return voroodi;
	//solver.exportModel("voroodi.lp");
	IloNumArray sval(voroodi.en);
	solver.getValues(sval,voroodi.x);
//	cerr<<voroodi.x<<endl;
	//cerr<<"values find is: "<<endl<<sval<<endl;
//	cerr<<tim<<endl;
//	cerr<<sval<<endl;
//	cerr<<sval[0]<<endl;
	//getch();
	solver.end();
	//cerr<<sval<<endl;
	//getch();
	//cerr<<tim<<endl;
	for(int i=0;i<sval.getSize();i++)
		if((sval[i]>0.01)&&(sval[i]<0.99)){
		//	cerr<<i<<' '<<sval[i]<<endl;
			ok=false;
			break;
		}	
	if(ok){
		//cerr<<"i found a solution with objval=  "<<x<<endl;
	//	cerr<<sval<<endl;
		//getch();
		best=min(best,x);
		return voroodi;
	}
	for(int i=0;i<sval.getSize();i++)
		if((sval[i]>0.01)&&(sval[i]<0.99)){
			typ num1;
			num1.en=voroodi.en;
			num1.x=voroodi.x;
			num1.mod=IloModel(num1.en);
			IloModel::Iterator iter(voroodi.mod);
			 while (iter.ok()) {
				  num1.mod.add(*iter);
				++iter;
			}			
		//	cerr<<"sval number "<<i<<" is not integer"<<endl;
			num1.mod.add(IloConstraint(num1.x[i]==0));
			num1=rec(num1,g);
			double u=getval(num1);
			if(u==g.colors.size())
				return num1;
			typ num2;
			num2.en=voroodi.en;
			num2.x=voroodi.x;
			num2.mod=IloModel(num2.en);
			IloModel::Iterator iter2(voroodi.mod);
			 while (iter2.ok()) {
			//	 if (((*iter2).asConstraint().getImpl())||((*iter2).asObjective.getImpl()))
				  num2.mod.add(*iter2);
				++iter2;
			}
			IloCplex solver2(num2.mod);
			//num2.mod.remove(IloConstraint(num2.x[i]==0));
			num2.mod.add(IloConstraint(num2.x[i]==1));
		//	solver2.exportModel("num2.lp");
		//	cerr<<"the second model i made is"<<endl<<num2.mod<<endl<<endl;
		//	getch();
			//solver2.exportModel("after.lp");	
			num2=rec(num2,g);
			double v=getval(num2);
			solver2.end();
			if(u<v)
				return num1;
			else
				return num2;
		}
	//	else
		//	voroodi.mod.add(IloConstraint(voroodi.x[i]==sval[i]));
}


int main(){
	for(int ff=21;ff<=30;ff++){
		string s1="graph"+to_string(ff)+".txt";
		string s2="color"+to_string(ff)+".txt";
		fin.open(s1);
		qin.open(s2);
		IloEnv env;
		graph g;
		//IloModel model(env);	
		//IloModel md=model;
		/*IloNumVar x2=IloNumVar(env,0,IloInfinity);
		IloRange sum_to_one=IloRange(env,1,1);
		IloObjective obj=IloMaximize(env,0);
		sum_to_one.setLinearCoef(x1,1);
		sum_to_one.setLinearCoef(x2,1);
		obj.setLinearCoef(x1,2);
		obj.setLinearCoef(x2,1);
		model.add(obj);
		model.add(sum_to_one);
		IloCplex solver(model);
		solver.setOut(env.getNullStream());
		solver.solve();
		cout<<solver.getObjValue()<<endl;
		getch();*/
	 // Minisat::Solver solver;
	 // auto A = solver.newVar();
	  timelimit=false;
	  start=clock();
	  g.read();
	  best=g.v;
	  typ model(env,g);
	  typ md=model;
	  //IloRange ran(env,0,g.v-1);
	  IloExpr expr(env);
	  map<int,set<int> >::iterator it=g.adj.begin();
	  while(it!=g.adj.end()){
		  expr+=model.x[it->first];
		  it++;
	  }
	  IloObjective obj=IloMinimize(env,expr);
	  expr.end();
  
	  for(map<int,set<int> >::iterator it=g.colors.begin();it!=g.colors.end();it++){
		  IloExpr expr(env);
		  for(set<int>::iterator st=it->second.begin();st!=it->second.end();st++)
			  expr+=model.x[*st];
		  model.mod.add(IloConstraint(expr>=1));
		  expr.end();
	  }

	  model.mod.add(obj);
	  double mini=0;
	  int bar=0;
	  int cc=0;
	  IloNumArray jav;
  
	//  cerr<<jav<<endl;
	 // cerr<<"halle"<<endl;
	 typ nahayi=rec(model,g);
	 IloCplex solver(nahayi.mod);
	 solver.solve();
	 IloNumArray sval(env);
	 solver.getValues(sval,model.x);
	 //cerr<<nahayi.mod<<endl;
	//cerr<<sval<<endl;
	 int ans=getval(nahayi);
	 fout<<"test "<<ff<<": ";
	 fout<<g.v<<' '<<g.numedges()<<' ';
	 if(timelimit)
		 fout<<"timelimit"<<endl;
	 else
		 fout<<gettime()<<' '<<ans<<endl;
	// cerr<<"ended!"<<endl;
	  fin.close();
	  qin.close();
	//  getch();
	}
//	getch();
	return 0;
}
