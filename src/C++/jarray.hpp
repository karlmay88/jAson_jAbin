#ifndef JARRAY_L_H
#define JARRAY_L_H
#include <string>
#include <stdio.h>
#include <vector>
template<typename TT> class jarray_array;
class jarray_parsekey{
private:
protected:
public:
  std::string operator()(size_t &brc, FILE *fp,bool isbinary=true)const;
};
template<typename T>
class jarray_parsedata{
private:
protected:
public:
  std::string operator()(size_t &brc, FILE *fp,bool isbinary=true)const;
  void* operator()(long long int n,size_t &brc, FILE *fp,bool isbinary=true);
};
class jarray_descriptor{
private:
protected:
  std::string data;
public:
  jarray_descriptor(){};
  jarray_descriptor(const std::string in):data(in){};
  jarray_descriptor(const jarray_descriptor &in):data(in.data){};
  jarray_descriptor(jarray_descriptor &&in):data(in.data){};
  jarray_descriptor(size_t &brc,FILE *fp,bool isbinary=true);
  jarray_descriptor& operator=(jarray_descriptor &in){this->data=in.data;return *this;}
  jarray_descriptor& operator=(jarray_descriptor &&in){this->data=in.data;return *this;}
  void write(FILE *fp,bool isbinary=true);
  std::string get()const{return(this->data);};
};
class jarray_type{
private:
protected:
  std::string data;
public:
  jarray_type(){};
  jarray_type(size_t &brc,FILE *fp,bool isbinary=true);
  jarray_type(std::string x):data(x){};
  void write(FILE *fp,bool isbinary=true);
  std::string get()const{return(this->data);};
};
template<class TT>
class jarray_bits{
private:
protected:
  TT data=(TT)0;
public:
  jarray_bits(){};
  jarray_bits(size_t &brc,FILE *fp,bool isbinary=true);
  jarray_bits(TT x):data(x){};
  void write(FILE *fp,bool isbinary=true);
  TT get()const{return(this->data);};
};
template<class TT>
class jarray_ndims{
private:
protected:
  TT data=0;
public:
  jarray_ndims(){};
  jarray_ndims(size_t &brc,FILE *fp,bool isbinary=true);
  jarray_ndims(TT x):data(x){};
  void write(FILE *fp,bool isbinary=true);
  TT get()const{return(this->data);};
};
template<class TT>
class jarray_dims{
private:
protected:
  std::vector<TT> data;
public:
  ~jarray_dims(){};
  jarray_dims(){};
  jarray_dims(TT ndims, size_t &brc,FILE *fp,bool isbinary=true);
  jarray_dims(std::vector<TT> &x):data(x){};
  void write(FILE *fp,bool isbinary=true);
  std::vector<TT> get()const{return(this->data);};
  TT prod()const{
    TT x=1;
    for(int i=0;i<this->data.size();++i){x*=this->data[i];}
    return(x);
  }
};
class jarray_data{
private:
protected:
  void* data=nullptr;
public:
  jarray_data(){};
  template<typename TT>
  jarray_data(jarray_type &type,jarray_bits<TT> &bits,jarray_dims<TT> &dims,size_t &brc,FILE *fp,bool isbinary=true);
  jarray_data(void *x){this->data=x;}
  jarray_data(jarray_data &in){this->data=in.data;};
  jarray_data(jarray_data &&in){this->data=in.data;in.data=nullptr;}
  jarray_data& operator=(jarray_data &in){this->data=in.data;return *this;}
  jarray_data& operator=(jarray_data &&in){this->data=in.data;in.data=nullptr;return *this;}
  ~jarray_data(){};
  const void* get()const{return(this->data);};
  template<class TT>
  void write(jarray_type &type,jarray_bits<TT> &bits,jarray_dims<TT> &dims,FILE *fp,bool isbinary=true);
  void* release(){void *out=this->data;this->data=nullptr;return(out);};
};
template<class TT>
class jarray_array{
private:
protected:
  jarray_type ty;
  jarray_ndims<TT> ndims;
  jarray_dims<TT> dims;
  jarray_bits<TT> bi;
  jarray_data da;
public:
  jarray_array(){};
  jarray_array(size_t &brc, FILE *fp,bool isbinary=true);
  jarray_array(void *data,std::vector<TT> dims,TT bits,std::string type):
    ty(type),bi(bits),ndims((TT)dims.size()),dims(dims),da(data){};
  jarray_array& operator=(jarray_array &in){
    this->ty=in.ty;this->ndims=in.ndims;this->dims=in.dims;this->bi=in.bi;this->da=in.da;
    return *this;
  }
  jarray_array& operator=(jarray_array &&in){
    this->ty=in.ty;this->ndims=in.ndims;this->dims=in.dims;this->bi=in.bi;this->da=in.da;
    return *this;
  }
  void write(FILE *fp,bool isbinary);
  ~jarray_array()=default;
  std::string gettype()const{return(this->ty.get());}
  long long int getndims()const{return(this->ndims.get());}
  long long int getbits()const{return(this->bi.get());}
  std::vector<TT> getdims()const{return(this->dims.get());}
  void* release(){return(this->da.release());}
};
template<class TT>
class jarray_map{
private:
protected:
  std::string name;
  jarray_descriptor *de=nullptr;
  std::vector<jarray_map*> data;
  jarray_array<TT> *array=nullptr;
  std::string file;
public:
  jarray_map(std::string name, FILE *fp,bool isbinary=true);
  jarray_map(std::string name,size_t &brc, FILE *fp,bool isbinary=true);
  jarray_map(std::string name):name(name){};
  void add(jarray_array<TT> *in){this->array=in;in=nullptr;}
  jarray_map* add(jarray_map *in){
    this->data.push_back(in);
    return(this->data[this->data.size()-1]);
  }
  void add(const jarray_descriptor in){this->de=new jarray_descriptor(in);}
  void add(jarray_descriptor *in){this->de=in;in=nullptr;}
  jarray_map(jarray_map &in){};
  jarray_map(jarray_map &&in){};
  ~jarray_map(){
    for(long long int i=0;i<this->data.size();++i){
      delete this->data[i];
    }
    delete this->array;
    delete this->de;
  };
  void write(FILE *fp,bool isroot=false,bool isbinary=true);
  jarray_array<TT>* getarray(){return(this->array);}
  std::vector<jarray_map*>& getdata(){return(this->data);}
  const std::string& getname()const{return(this->name);}
};
std::string jarray_parsekey::operator()(size_t &brc,FILE *fp,bool isbinary)const{
  std::string emsg;
  try{
    std::string out;char cc;size_t x;
    //ommit blanks
    for(;;){
      if(isbinary){
	x=fread(&cc,sizeof(char),1,fp);
      }else{
	x=fscanf(fp,"%1c",&cc);
      }
      if(x!=1){emsg="error. reached end of data.";throw emsg;}
      if(cc=='}'){
	brc--;return(out);
      }else{
	if((int)cc==9 || (int)cc==10 || (int)cc==32) continue;
	out.push_back(cc);break;
      }
    }
    for(;;){
      if(isbinary){
	x=fread(&cc,sizeof(char),1,fp);
      }else{
	x=fscanf(fp,"%1c",&cc);
      }
      if(x!=1){emsg="error. reached end of data.";throw emsg;}
      if(cc==':'){
	if(isbinary){
	  x=fread(&cc,sizeof(char),1,fp);
	}else{
	  x=fscanf(fp,"%1c",&cc);
	}
	if(x!=1){emsg="error. reached end of data.";throw emsg;}
	if(out.size()==0){
	  emsg="error. found key of size zero.";throw emsg;
	}
	return(out);
      }
      out.push_back(cc);
    }
  }catch(std::string x){
    emsg="error when parsing key "+x;throw emsg;
  }
};
template<>
std::string jarray_parsedata<std::string>::operator()(size_t &brc, FILE *fp,bool isbinary)const{
  //starts with file pointer pointing BEHIND "{", ends with file pointer
  //pointing AT "}".
  std::string emsg;
  try{
    brc++;
    std::string out;char cc;size_t x;
    for(;;){
      if(isbinary){
	x=fread(&cc,sizeof(char),1,fp);
      }else{
	x=fscanf(fp,"%1c",&cc);
      }
      if(x!=1){emsg="error. reached end of data.";throw emsg;}
      if(int(cc)==9 || int(cc)==10 || int(cc)==32) continue;
      if(int(cc)=='}'){brc--;return(out);}
      out.push_back(cc);break;
    }
    for(;;){
      if(isbinary){
	x=fread(&cc,sizeof(char),1,fp);
      }else{
	x=fscanf(fp,"%1c",&cc);
      }
      if(x!=1){emsg="error. reached end of data.";throw emsg;}
      if(cc=='}'){
	brc--;return(out);
      }
      out.push_back(cc);
    }
  }catch(std::string x){
    emsg="error when parsing string data. "+x;throw emsg;
  }
};
#undef xxxx
#define xxxx "%lld"
#undef yyyy
#define yyyy long long int
template<>
void* jarray_parsedata<long long int>::operator()(long long int n,size_t &brc, FILE *fp,bool isbinary){
  //starts with filepointer pointing behind "{", ends with file pointer pointing at "}"
#include "in_jarray_1.cpp"
}
#undef xxxx
#define xxxx "%ld"
#undef yyyy
#define yyyy long int
template<>
void* jarray_parsedata<long int>::operator()(long long int n,size_t &brc, FILE *fp,bool isbinary){
  //starts with filepointer pointing behind "{", ends with file pointer pointing at "}"
#include "in_jarray_1.cpp"
}
#undef xxxx
#define xxxx "%d"
#undef yyyy
#define yyyy int
template<>
void* jarray_parsedata<int>::operator()(long long int n,size_t &brc, FILE *fp,bool isbinary){
  //starts with filepointer pointing behind "{", ends with file pointer pointing at "}"
#include "in_jarray_1.cpp"
}
#undef xxxx
#define xxxx "%lf"
#undef yyyy
#define yyyy double
template<>
void* jarray_parsedata<double>::operator()(long long int n,size_t &brc, FILE *fp,bool isbinary){
  //starts with filepointer pointing behind "{", ends with file pointer pointing at "}"
#include "in_jarray_1.cpp"
}
#undef xxxx
#define xxxx "%f"
#undef yyyy
#define yyyy float
template<>
void* jarray_parsedata<float>::operator()(long long int n,size_t &brc, FILE *fp,bool isbinary){
  //starts with filepointer pointing behind "{", ends with file pointer pointing at "}"
#include "in_jarray_1.cpp"
}
#undef xxxx
#define xxxx "%c"
#undef yyyy
#define yyyy char
template<>
void* jarray_parsedata<char>::operator()(long long int n,size_t &brc, FILE *fp,bool isbinary){
  //starts with filepointer pointing behind "{", ends with file pointer pointing at "}"
#include "in_jarray_1.cpp"
}
jarray_descriptor::jarray_descriptor(size_t &brc,FILE *fp,bool isbinary){
  std::string emsg;
  try{
    jarray_parsedata<std::string> x;this->data=x(brc,fp,isbinary);
  }catch(std::string x){
    emsg="error while evaluating descriptor: "+x;throw emsg;
  }
}
jarray_type::jarray_type(size_t &brc, FILE *fp,bool isbinary){
  std::string emsg;
  try{
    jarray_parsedata<std::string> x;this->data=x(brc,fp,isbinary);
    if(this->data!="r" && this->data!="i" && this->data!="c" && this->data!="s"){
      emsg="error. parenthesis space not sound. must contain only character \"c\", \"r\" \"i\" or \"s\".";throw emsg;
    }
  }catch(std::string x){
    emsg="error while evaluating ty: "+x;throw emsg;
  }
};
template<class TT>
jarray_bits<TT>::jarray_bits(size_t &brc, FILE *fp,bool isbinary){
  std::string emsg;
  try{
    jarray_parsedata<TT> x;
    void *y=x((long long int)1,brc,fp,isbinary);
    this->data=*(TT*)y;free(y);
    if(this->data!=64 && this->data!=32){
      emsg="error. supported bit are 64 and 32. Found "+std::to_string(this->data)+". ";throw emsg;
    }
  }catch(std::string x){
    emsg="error while evaluating bi: "+x;throw emsg;
  }
};
template<class TT>
jarray_ndims<TT>::jarray_ndims(size_t &brc, FILE *fp,bool isbinary){
  std::string emsg;
  try{
    jarray_parsedata<TT> x;
    void *y=x((long long int)1,brc,fp,isbinary);
    this->data=*(TT*)y;free(y);
    if(this->data<0 || this->data>3){
      emsg="error. ndims can only be 0(for scalar), 1(for vector), 2(for matrix) or 3(for 3d array).";throw emsg;
    }
  }catch(std::string x){
    emsg="error while evaluating nd: "+x;throw emsg;
  }
};
template<class TT>
jarray_dims<TT>::jarray_dims(TT ndims, size_t &brc, FILE *fp,bool isbinary){
  std::string emsg;
  try{
    if(ndims==0) return;
    jarray_parsedata<TT> x;
    TT *p=(TT*)x((long long int)ndims,brc,fp,isbinary);
    for(TT i=0;i<ndims;++i){
      this->data.push_back(p[i]);
      if(p[i]<1){emsg="error. dimensions must be greater than zero.";throw emsg;}
    }
    free(p);
  }catch(std::string x){
    emsg="error while evaluating di: "+x;throw emsg;
  }
};
template<class TT>
jarray_data::jarray_data(jarray_type &type,jarray_bits<TT> &bits,jarray_dims<TT> &dims, size_t &brc, FILE *fp,bool isbinary){
  std::string emsg;
  try{
    long long int sz=(long long int)dims.prod();
    if(type.get()=="r"){
      if(bits.get()==64){
	jarray_parsedata<double> x;this->data=x((long long int)sz,brc,fp,isbinary);
      }else if(bits.get()==32){
	jarray_parsedata<float> x;this->data=x((long long int)sz,brc,fp,isbinary);
      }else{
	emsg="error. bi must be 32 or 64.";throw emsg;
      }
    }else if(type.get()=="i"){
      if(bits.get()==64){
	jarray_parsedata<long long int> x;this->data=x((long long int)sz,brc,fp,isbinary);
      }else if(bits.get()==32){
	jarray_parsedata<long int> x;this->data=x((long long int)sz,brc,fp,isbinary);
      }else{
	emsg="error. bi must be 32 or 64.";throw emsg;
      }
    }else if(type.get()=="s"){
      if(bits.get()==32){
	jarray_parsedata<int> x;this->data=x((long long int)sz,brc,fp,isbinary);
      }else{
	emsg="error. bi must be 32.";throw emsg;
      }
    }else if(type.get()=="c"){
      if(bits.get()==8){
	jarray_parsedata<char> x;this->data=x((long long int)sz,brc,fp,isbinary);
      }else{
	emsg="error. bi must be 8.";throw emsg;
      }
    }
  }catch(std::string x){
    emsg="error while evaluating data: "+x;throw emsg;
  }
};
template<class TT>
jarray_array<TT>::jarray_array(size_t &brc, FILE *fp,bool isbinary){
  std::string emsg;
  try{
    size_t brcl=brc;
    jarray_parsekey parse;
    std::string key;
    std::vector<std::string> keys;
    key=parse(brc,fp,isbinary);
    if(key=="ty"){
      this->ty=jarray_type(brc,fp,isbinary);
    }else{
      emsg="error in \"array\". first key must be \"ty\" but found \""+key+"\".";throw emsg;
    }
    if(brc!=brcl){
      emsg="error in \"array\" after evaluating key \""+key+"\". parenthesis are not balanced.";throw emsg;
    }
    key=parse(brc,fp,isbinary);
    if(key=="bi"){
      this->bi=jarray_bits<TT>(brc,fp,isbinary);
    }else{
      emsg="error in \"array\". second key must be \"bi\" but found \""+key+"\".";throw emsg;
    }
    if(brc!=brcl){
      emsg="error in \"array\" after evaluating key \""+key+"\". parenthesis are not balanced.";throw emsg;
    }
    key=parse(brc,fp,isbinary);
    if(key=="nd"){
      this->ndims=jarray_ndims<TT>(brc,fp,isbinary);
    }else{
      emsg="error in \"array\". third key must be \"nd\" but found \""+key+"\".";throw emsg;
    }
    if(brc!=brcl){
      emsg="error in \"array\" after evaluating key \""+key+"\". parenthesis are not balanced.";throw emsg;
    }
    key=parse(brc,fp,isbinary);
    if(key=="di"){
      this->dims=jarray_dims<TT>(this->ndims.get(),brc,fp,isbinary);
    }else{
      emsg="error in \"array\". fourth key must be \"di\" but found \""+key+"\".";throw emsg;
    }
    if(brc!=brcl){
      emsg="error in \"array\" after evaluating key \""+key+"\". parenthesis are not balanced.";throw emsg;
    }
    key=parse(brc,fp,isbinary);
    if(key=="da"){
      this->da=jarray_data(this->ty,this->bi,this->dims,brc,fp,isbinary);
    }else{
      emsg="error in \"array\". second key must be \""+key+"\".";throw emsg;
    }
    if(brc!=brcl){
      emsg="error in \"array\" after evaluating key \""+key+"\". parenthesis are not balanced.";throw emsg;
    }
  }catch(std::string x){
    emsg="error when evaluating \"array\". "+x;throw emsg;
  }
}
template<class TT>
jarray_map<TT>::jarray_map(std::string name,size_t &brc, FILE *fp,bool isbinary):name(name){
  std::string emsg;
  try{
    size_t brcl=brc;
    jarray_parsekey parse;
    std::string key;
    std::vector<std::string> keys;
    char cc;int x=0;
    for(;;){
      key=parse(brc,fp,isbinary);
      if(brc<brcl) break;
      if(brc!=brcl){
	emsg="error in file "+file+". parenthesis are not balanced. "+std::to_string(int(brc));throw emsg;
      }
      if(key.size()==0) continue;
      for(auto x : keys){
	if(x==key){emsg="error. key \""+key+"\" not unique.";throw emsg;}
      }
      keys.push_back(key);
      if(key=="array"){
	brc++;
	this->array=new jarray_array<TT>(brc,fp,isbinary);
      }else if(key=="de"){
	//no de++ for key value pairs
	this->de=new jarray_descriptor(brc,fp,isbinary);
      }else{
	brc++;
	this->data.push_back(new jarray_map<TT>(key,brc,fp,isbinary));
      }
    }
  }catch(std::string k){
    emsg="error when evaluating sub-node \""+this->name+"\". "+k;throw emsg;
  }
}
template<class TT>
jarray_map<TT>::jarray_map(std::string name, FILE *fp,bool isbinary):name(name){
  std::string emsg;
  try{
    if(fp==nullptr){emsg="error. file not open.";throw emsg;}
    char cc;size_t brc=0;size_t x;
    if(isbinary){x=fread(&cc,sizeof(char),1,fp);}else{x=fscanf(fp,"%1c",&cc);}
    if(x!=1){emsg="error. reached end of data.";throw emsg;}
    if(cc!='{'){emsg="error. file must start with a \"{\".";throw emsg;}
    brc++;
    jarray_parsekey parse;
    std::string key;
    std::vector<std::string> keys;
    int k=0;
    for(;;){
      //parse will brc-- if it encounters "}" and will return string of length zero.
      key=parse(brc,fp,isbinary);
      if(brc<1) break;
      if(brc!=1){
	emsg="error in file "+file+". parenthesis are not balanced. "+std::to_string(brc);throw emsg;
      }
      if(key.size()==0) continue;
      for(auto x : keys){
	if(x==key){emsg="error. key \""+key+"\" not unique.";throw emsg;}
      }
      keys.push_back(key);
      if(key=="de"){
	//no de++ for key value pairs
	this->de=new jarray_descriptor(brc,fp,isbinary);
      }else if(key=="array"){
	brc++;
	this->array=new jarray_array<TT>(brc,fp,isbinary);
      }else{
	brc++;
	this->data.push_back(new jarray_map<TT>(key,brc,fp,isbinary));
      }
      k++;
    }
  }catch(std::string x){
    emsg="error in node \""+this->name+"\". "+x;throw emsg;
  }
};
void jarray_descriptor::write(FILE *fp,bool isbinary){
  std::string emsg;
  try{
    char x1[]="de"; char co=':'; char op='{'; char cl='}';
    if(isbinary){
      fwrite(&x1,2,1,fp);
      fwrite(&co,1,1,fp);
      fwrite(&op,1,1,fp);
      for(long long int i=0;i<this->data.size();++i){
	fwrite(&this->data[i],sizeof(char),1,fp);
      }
      fwrite(&cl,1,1,fp);
    }else{
      fprintf(fp,"%s%c%c%s%c\n",x1,co,op,this->data.c_str(),cl);
    }
  }catch(std::string x){
    emsg="error while writing to file. "+x;throw emsg;
  }
}
void jarray_type::write(FILE *fp,bool isbinary){
  std::string emsg;
  try{
    char x1[]="ty",co=':',op='{',cl='}';
    if(isbinary){
      fwrite(&x1,2,1,fp);
      fwrite(&co,1,1,fp);
      fwrite(&op,1,1,fp);
      fwrite(&this->data[0],sizeof(char),1,fp);
      fwrite(&cl,1,1,fp);
    }else{
      fprintf(fp,"%s%c%c%s%c\n",x1,co,op,this->data.c_str(),cl);
    }
  }catch(std::string x){
    emsg="error while writing to file. "+x;throw emsg;
  }
}
#undef xxxx
#define xxxx \
  std::string emsg;\
  try{\
    char x1[]="bi",co=':',op='{',cl='}';\
    if(isbinary){\
      fwrite(&x1,2,1,fp);\
      fwrite(&co,1,1,fp);\
      fwrite(&op,1,1,fp);\
      fwrite(&this->data,sizeof(yyyy1),1,fp);\
      fwrite(&cl,1,1,fp);\
    }else{\
      fprintf(fp,yyyy,x1,co,op,this->data,cl);\
    }\
  }catch(std::string x){\
    emsg="error while writing to file. "+x;throw emsg;\
  }

template<>
void jarray_bits<long long int>::write(FILE *fp,bool isbinary){
#undef yyyy
#define yyyy "%s%c%c%lld%c\n"
#undef yyyy1
#define yyyy1 long long int
  xxxx;
}
template<>
void jarray_bits<long int>::write(FILE *fp,bool isbinary){
#undef yyyy
#define yyyy "%s%c%c%ld%c\n"
#undef yyyy1
#define yyyy1 long int
  xxxx;
}
template<>
void jarray_bits<int>::write(FILE *fp,bool isbinary){
#undef yyyy
#define yyyy "%s%c%c%d%c\n"
#undef yyyy1
#define yyyy1 int
  xxxx;
}

#undef xxxx
#define xxxx \
  std::string emsg;\
  try{\
    char x1[]="nd",co=':',op='{',cl='}';\
    if(isbinary){\
      fwrite(&x1,2,1,fp);\
      fwrite(&co,1,1,fp);\
      fwrite(&op,1,1,fp);\
      fwrite(&this->data,sizeof(yyyy1),1,fp);\
      fwrite(&cl,1,1,fp);\
    }else{\
      fprintf(fp,yyyy,x1,co,op,this->data,cl);\
    }\
  }catch(std::string x){\
    emsg="error while writing to file. "+x;throw emsg;\
  }

template<>
void jarray_ndims<long long int>::write(FILE *fp, bool isbinary){
#undef yyyy
#define yyyy "%s%c%c%lld%c\n"
#undef yyyy1
#define yyyy1 long long int
  xxxx;
}
template<>
void jarray_ndims<long int>::write(FILE *fp, bool isbinary){
#undef yyyy
#define yyyy "%s%c%c%ld%c\n"
#undef yyyy1
#define yyyy1 long int
  xxxx;
}
template<>
void jarray_ndims<int>::write(FILE *fp, bool isbinary){
#undef yyyy
#define yyyy "%s%c%c%d%c\n"
#undef yyyy1
#define yyyy1 int
  xxxx;
}

#undef xxxx
#define xxxx\
  std::string emsg;\
  try{\
    char x1[]="di",co=':',op='{',cl='}';\
    if(isbinary){\
      fwrite(&x1,2,1,fp);\
      fwrite(&co,1,1,fp);\
      fwrite(&op,1,1,fp);\
      for(long long int i=0;i<this->data.size();++i){\
	fwrite(&this->data[i],sizeof(yyyy1),1,fp);\
      }\
      fwrite(&cl,1,1,fp);\
    }else{\
      if(this->data.size()==0){\
	emsg="error. dimension of zero not supported.";throw emsg;\
      }else if(this->data.size()==1){\
	fprintf(fp,yyyy,x1,co,op,this->data[0],cl);\
      }else{\
	fprintf(fp,yyyy2,x1,co,op,this->data[0]);\
	for(int i=1;i<this->data.size()-1;++i){\
	  fprintf(fp,yyyy3,this->data[i]);\
	}\
	fprintf(fp,yyyy4,this->data[this->data[this->data.size()-1]],cl);\
      }\
    }\
  }catch(std::string x){\
    emsg="error while writing to file. "+x;throw emsg;\
  }

template<>
void jarray_dims<long long int>::write(FILE *fp,bool isbinary){
#undef yyyy
#define yyyy "%s%c%c%lld%c\n"
#undef yyyy1
#define yyyy1 long long int
#undef yyyy2
#define yyyy2 "%s%c%c%lld\n"
#undef yyyy3
#define yyyy3 "%lld\n"
#undef yyyy4
#define yyyy4 "%lld%c\n"
  xxxx;
}
template<>
void jarray_dims<long int>::write(FILE *fp,bool isbinary){
#undef yyyy
#define yyyy "%s%c%c%ld%c\n"
#undef yyyy1
#define yyyy1 long int
#undef yyyy2
#define yyyy2 "%s%c%c%ld\n"
#undef yyyy3
#define yyyy3 "%ld\n"
#undef yyyy4
#define yyyy4 "%ld%c\n"
  xxxx;
}
template<>
void jarray_dims<int>::write(FILE *fp,bool isbinary){
#undef yyyy
#define yyyy "%s%c%c%d%c\n"
#undef yyyy1
#define yyyy1 int
#undef yyyy2
#define yyyy2 "%s%c%c%d\n"
#undef yyyy3
#define yyyy3 "%d\n"
#undef yyyy4
#define yyyy4 "%d%c\n"
  xxxx;
}

#undef xxxx
#undef yyyy
#undef yyyy1
#undef yyyy2
#undef yyyy3
#undef yyyy4
template<class TT>
void jarray_data::write(jarray_type &type,jarray_bits<TT> &bits,jarray_dims<TT> &dims,FILE *fp,bool isbinary){
  std::string emsg;
  try{
    char x1[]="da",co=':',op='{',cl='}';
    long long int sz=(long long int)dims.prod();
    if(isbinary){
      fwrite(&x1,2,1,fp);
      fwrite(&co,1,1,fp);
      fwrite(&op,1,1,fp);
      if(type.get()=="r"){
	if(bits.get()==64){
	  double *p=(double*)this->data;
	  for(long long int i=0;i<sz;++i){
	    fwrite(&p[i],sizeof(double),1,fp);
	  }
	}else if(bits.get()==32){
	  float *p=(float*)this->data;
	  for(long long int i=0;i<sz;++i){
	    fwrite(&p[i],sizeof(float),1,fp);
	  }
	}else{
	  emsg="error. bits must be 32 or 64.";throw emsg;
	}
      }else if(type.get()=="i"){
	if(bits.get()==64){
	  long long int *p=(long long int*)this->data;
	  for(long long int i=0;i<sz;++i){
	    fwrite(&p[i],sizeof(long long int),1,fp);
	  }
	}else if(bits.get()==32){
	  long int *p=(long int*)this->data;
	  for(long long int i=0;i<sz;++i){
	    fwrite(&p[i],sizeof(long int),1,fp);
	  }
	}else{
	  emsg="error. bits must be 32 or 64.";throw emsg;
	}
      }else if(type.get()=="c"){
	if(bits.get()==8){
	  char *p=(char*)this->data;
	  for(long long int i=0;i<sz;++i){
	    fwrite(&p[i],sizeof(char),1,fp);
	  }
	}else{
	  emsg="error. bits must be 8.";throw emsg;
	}
      }
      fwrite(&cl,1,1,fp);
    }else{
#undef xxxx
#define xxxx \
      if(sz==0){\
	fprintf(fp,"%s%c%c%c\n",x1,co,op,cl);\
      }else if(sz==1){\
	fprintf(fp,yyyy1,x1,co,op,p[0],cl);\
      }else{\
	fprintf(fp,yyyy2,x1,co,op,p[0]);\
	for(long long int i=1;i<sz-1;++i){\
	  fprintf(fp,yyyy3,p[i]);\
	}\
	fprintf(fp,yyyy4,p[sz-1],cl);\
      }
      if(type.get()=="r"){
	if(bits.get()==64){
	  double *p=(double*)this->data;
#undef yyyy1
#define yyyy1 "%s%c%c%.17g%c\n"
#undef yyyy2
#define yyyy2 "%s%c%c%.17g\n"
#undef yyyy3
#define yyyy3 "%.17g\n"
#undef yyyy4
#define yyyy4 "%.17g%c\n"
	  xxxx;
	}else if(bits.get()==32){
	  float *p=(float*)this->data;
	  xxxx;
	}else{
	  emsg="error. bits must be 32 or 64.";throw emsg;
	}
      }else if(type.get()=="i"){
	if(bits.get()==64){
	  long long int *p=(long long int*)this->data;
#undef yyyy1
#define yyyy1 "%s%c%c%lld%c\n"
#undef yyyy2
#define yyyy2 "%s%c%c%lld\n"
#undef yyyy3
#define yyyy3 "%lld\n"
#undef yyyy4
#define yyyy4 "%lld%c\n"
	  xxxx;
	}else if(bits.get()==32){
	  long int *p=(long int*)this->data;
#undef yyyy1
#define yyyy1 "%s%c%c%ld%c\n"
#undef yyyy2
#define yyyy2 "%s%c%c%ld\n"
#undef yyyy3
#define yyyy3 "%ld\n"
#undef yyyy4
#define yyyy4 "%ld%c\n"
	  xxxx;
	}else{
	  emsg="error. bits must be 32 or 64.";throw emsg;
	}
    }else if(type.get()=="c"){
      if(bits.get()==8){
	  char *p=(char*)this->data;
	  if(sz==0){
	    fprintf(fp,"%s%c%c%c\n",x1,co,op,cl);
	  }else if(sz==1){
	    fprintf(fp,"%s%c%c%c%c",x1,co,op,p[0],cl);
	  }else{
	    long long int n=0;
	    fprintf(fp,"%s%c%c",x1,co,op);
	    for(long long int i=0;i<sz;++i){
	      for(;;){
		if(p[i]!='\0'){
		  fprintf(fp,"%c",p[i]);++i;
		}else{
		  if(i==sz-1){
		    fprintf(fp,"%c%c\n",p[i],cl);
		  }else{
		    fprintf(fp,"\n");break;
		  }
		}
	      }
	    }
	  }
	}else{
	  emsg="error. bits must be 8.";throw emsg;
	}
      }
    }
  }catch(std::string x){
      emsg="error when writing to file. "+x;throw emsg;
  }
}
template<class TT>
void jarray_array<TT>::write(FILE *fp,bool isbinary){
  std::string emsg;
  try{
    char x1[]="array";char co=':'; char op='{'; char cl='}';
    if(isbinary){
      fwrite(&x1,5,1,fp);
      fwrite(&co,1,1,fp);
      fwrite(&op,1,1,fp);
    }else{
      fprintf(fp,"%s%c%c\n",x1,co,op);
    }
    this->ty.write(fp,isbinary);
    this->bi.write(fp,isbinary);
    this->ndims.write(fp,isbinary);
    this->dims.write(fp,isbinary);
    this->da.write(this->ty,this->bi,this->dims,fp,isbinary);
    if(isbinary){
      fwrite(&cl,1,1,fp);
    }else{
      fprintf(fp,"%c\n",cl);
    }
  }catch(std::string x){
    emsg="error when writing array. "+x;throw emsg;
  }
}
template<class TT>
void jarray_map<TT>::write(FILE *fp,bool isroot,bool isbinary){
  std::string emsg;
  try{
    char co=':'; char op='{'; char cl='}';
    if(!isroot){
      if(isbinary){
	fwrite(this->name.c_str(),1,this->name.size(),fp);
	fwrite(&co,1,1,fp);
	fwrite(&op,1,1,fp);
      }else{
	fprintf(fp,"%s%c%c\n",this->name.c_str(),co,op);
      }
    }else{
      if(isbinary){
	fwrite(&op,1,1,fp);
      }else{
	fprintf(fp,"%c\n",op);
      }
    }
    if(this->de!=nullptr){
      this->de->write(fp,isbinary);
    }
    for(long long int i=0;i<this->data.size();++i){
      this->data[i]->write(fp,false,isbinary);
    }
    if(this->array!=nullptr){
      this->array->write(fp,isbinary);
    }
    if(isbinary){
      fwrite(&cl,1,1,fp);
    }else{
      fprintf(fp,"%c\n",cl);
    }
  }catch(std::string x){
    emsg="error while writing to file: "+x;throw emsg;
  }
}
template class jarray_map<long long int>;
template class jarray_map<long int>;
template class jarray_map<int>;
#endif
