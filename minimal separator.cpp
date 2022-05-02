#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include <cstring>
#include <sstream>
#include <map>
#include <conio.h>

using namespace std;

ifstream fin;


int num=1;
class graph{
    public:
    int v;
    map<int,set<int> >adj;
    set<pair<int,int> >degrees;
    map<int,int> c;
    map<int,int> ham;
    graph(int n,map<int,set<int> >a){
      v=n;
      adj=a;
      ham.clear();
      c.clear();
      degrees.clear();
      for(int i=0;i<v;i++)
        degrees.insert(make_pair(adj[i].size(),i));
    }
    graph(){};
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
          for(int i=0;i<adj.size();i++)
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
          for(int i=0;i<v;i++)
            degrees.insert(make_pair(adj[i].size(),i));
          break;
        }
      }
    }
    void read2(){
      long e;
      cin>>v>>e;
      degrees.clear();
      for(int i=0;i<adj.size();i++)
        adj[i].clear();
      for(int i=0;i<e;i++){

        int p,q;
        cin>>p>>q;
        adj[p].insert(q);
        adj[q].insert(p);
      }
      return;
    }


};

graph operator-(graph g,long c){
    graph ans=g;
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
  set<int> fehrest;
  fehrest.clear();
  fehrest.insert(a);
  for(set<int>::iterator i=fehrest.begin();i!=fehrest.end();i++){
    set<int>::iterator it=g.adj[*i].begin();
    while(it!=g.adj[*i].end()){
      if(!checked[*it]){
        fehrest.insert(*it);
        checked[*it]=true;
      }
      it++;
    }
  }
  return fehrest;
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
int main(){
 // Minisat::Solver solver;
 // auto A = solver.newVar();

  graph g;
  g.read2();
  set<int> s1{1,2};
  set<int> s2{4,5};
 // cerr<<bfs(g,s2).size()<<endl;
  vector<int> f=minimal_sep(g,s1,s2);

  for(int i=0;i<f.size();i++)
    cout<<f[i]<<endl;
    //}
    return 0;
}
