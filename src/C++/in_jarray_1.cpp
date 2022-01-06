  std::string emsg;
  try{
    char cc;size_t x;
    void *out=malloc(sizeof(yyyy)*n);
    yyyy *tmp=(yyyy*)out;
    for(size_t i=0;i<n;++i){
      yyyy in;
      if(isbinary){
	x=fread(&in,sizeof(yyyy),1,fp);
      }else{
	x=fscanf(fp,xxxx,&in);
      }
      if(x!=1){emsg="error. reached end of data 1. ";throw emsg;}
      try{
	tmp[i]=in;
      }catch(...){
	emsg="error when reading array value from file.";throw emsg;
      }
    }
    if(isbinary){
      x=fread(&cc,sizeof(char),1,fp);
    }else{
      x=fscanf(fp,"%1c",&cc);
    }
    if(x!=1){emsg="error. reached end of data 2. ";throw emsg;}
    if(cc!='}'){emsg="error. variable space must end with a \"}\".";throw emsg;}
    return(out);
  }catch(std::string x){
    emsg="error when parsing array data. "+x;throw emsg;
  }
