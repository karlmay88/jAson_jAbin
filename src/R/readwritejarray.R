writejarrayascii <- function(out,file){
  writenode <- function(out,file){
    stopifnot(class(out)=="list");
    stopifnot(length(out)>0);
    stopifnot(length(names(out))>0);
    if("array" %in% names(out)){
      stopifnot(length(out)<=2);
      if(length(out)==2){
        stopifnot(names(out)==c("de","array"))
      }
    }
    na <- names(out)
    for(i in 1:length(out)){
      if(na[i]=="array"){
        cat("array:{","\n",append=T,file=ff,sep="");
        writearray(out[[i]],file);
        cat("}","\n",file=ff,append=T,sep="");
      }else if(na[i]=="de"){
        stopifnot(class(out[[i]])=="character")
        cat("de:{",out[[i]],"}","\n",append=T,file=ff,sep="");
      }else{
        cat(paste(na[i],":{",sep=""),"\n",append=T,file=ff,sep="");
        writenode(out[[i]],file);
        cat("}","\n",file=ff,append=T,sep="");
      }
    }
  }
  writearray <- function(out,file){
    stopifnot(class(out)[1] %in%
              c("numeric","integer","integer64","array","matrix"));
    if(class(out)[1] %in% c("numeric","integer","integer64")){
      stopifnot(length(out)>0);
      if(class(out)[1]=="numeric"){
        x <- "r"
      }else{
        x <- "i"
      }
      if(class(out)[1]=="integer64"){
        out <- as.integer(out);
      }
      cat(paste("ty:{",x,"}",sep=""),"\n",file=file,append=T,sep="")
      cat(paste("bi:{",64,"}",sep=""),"\n",file=file,append=T,sep="")
      cat(paste("nd:{",1,"}",sep=""),"\n",file=file,append=T,sep="")
      cat(paste("di:{",length(out),"}",sep=""),"\n",file=file,append=T,sep="");
      if(x=="i"){
        if(length(out)==1){
          cat(paste("da:{",out,"}",sep=""),"\n",file=file,append=T,sep="");
        }else if(length(out)==2){
          cat(paste("da:{",out[1],sep=""),"\n",file=file,append=T,sep="");
          cat(paste(out[2],"}",sep=""),"\n",file=file,append=T,sep="");
        }else{
          cat(paste("da:{",out[1],sep=""),"\n",file=file,append=T,sep="");
          for(i in 2:(length(out)-1)){
            cat(out[i],"\n",file=file,append=T,sep="");
          }
          cat(paste(out[length(out)],"}",sep=""),"\n",file=file,append=T,sep="");
        }
      }else{
        if(length(out)==1){
          cat(paste("da:{",formatC(out,digits=8,format="f"),"}",sep=""),"\n",file=file,append=T,sep="");
        }else if(length(out)==2){
          cat(paste("da:{",formatC(out[1],digits=8,format="f"),sep=""),"\n",file=file,append=T,sep="");
          cat(paste(formatC(out[2],digits=8,format="f"),"}",sep=""),"\n",file=file,append=T,sep="");
        }else{
          cat(paste("da:{",formatC(out[1],digits=8,format="f"),sep=""),"\n",file=file,append=T,sep="");
          for(i in 2:(length(out)-1)){
            cat(formatC(out[i],digits=8,format="f"),"\n",file=file,append=T,sep="");
          }
          cat(paste(formatC(out[length(out)],digits=8,format="f"),"}",sep=""),"\n",file=file,append=T,sep="");
        }
      }
    }else if(class(out)[1] %in% c("array","matrix")){
      stopifnot(length(dim(out))==2)
      stopifnot(sum(dim(out))>1)
      stopifnot(class(out[1,1])%in% c("integer","numeric","integer64"))
      if(class(out[1,1])=="numeric"){
        x <- "r"
      }else{
        x <- "i"
      }
      cat(paste("ty:{",x,"}",sep=""),"\n",file=file,append=T,sep="")
      cat(paste("bi:{",64,"}",sep=""),"\n",file=file,append=T,sep="")
      cat(paste("nd:{",length(dim(out)),"}",sep=""),"\n",file=file,append=T,sep="")
      cat(paste("di:{",dim(out)[1],sep=""),"\n",file=file,append=T,sep="");
      cat(paste(dim(out)[2],"}",sep=""),"\n",file=file,append=T,sep="");
      if(x=="r"){y=as.numeric(out);}else{y=as.integer(out);}
      if(x=="i"){
        if(length(out)==1){
          cat(paste("da:{",out,"}",sep=""),"\n",file=file,append=T,sep="");
        }else if(length(out)==2){
          cat(paste("da:{",out[1],sep=""),"\n",file=file,append=T,sep="");
          cat(paste(out[2],"}",sep=""),"\n",file=file,append=T,sep="");
        }else{
          cat(paste("da:{",out[1],sep=""),"\n",file=file,append=T,sep="");
          for(i in 2:(length(out)-1)){
            cat(out[i],"\n",file=file,append=T,sep="");
          }
          cat(paste(out[length(out)],"}",sep=""),"\n",file=file,append=T,sep="");
        }
      }else{
        if(length(out)==1){
          cat(paste("da:{",formatC(out,digits=8,format="f"),"}",sep=""),"\n",file=file,append=T,sep="");
        }else if(length(out)==2){
          cat(paste("da:{",formatC(out[1],digits=8,format="f"),sep=""),"\n",file=file,append=T,sep="");
          cat(paste(formatC(out[2],digits=8,format="f"),"}",sep=""),"\n",file=file,append=T,sep="");
        }else{
          cat(paste("da:{",formatC(out[1],digits=8,format="f"),sep=""),"\n",file=file,append=T,sep="");
          for(i in 2:(length(out)-1)){
            cat(formatC(out[i],digits=8,format="f"),"\n",file=file,append=T,sep="");
          }
          cat(paste(formatC(out[length(out)],digits=8,format="f"),"}",sep=""),"\n",file=file,append=T,sep="");
        }
      }
    }
  }
  unlink(file);
  ff=file;
  cat("{","\n",file=ff,append=T,sep="");
  writenode(out,file);
  cat("}","\n",file=ff,append=T,sep="");
}
writejarraybinary <- function(out,file){
  p <- match(c("sizeof.longlong","sizeof.long"),names(.Machine))
  if(all(is.na(p))){stop("error in writejarraybinary. missing longlong or long")}
  if(all(unlist(.Machine[p])!=8)){
    stop("error in writejarraybinary. missing size 8 for longlong or long")
  }
  require(mmap)
  writenode <- function(out,file){
    stopifnot(class(out)=="list");
    stopifnot(length(out)>0);
    stopifnot(length(names(out))>0);
    if("array" %in% names(out)){
      stopifnot(length(out)<=2);
      if(length(out)==2){
        stopifnot(names(out)==c("de","array"))
      }
    }
    na <- names(out)
    for(i in 1:length(out)){
      if(na[i]=="array"){
        xx <- "array:{"
        writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
        writearray(out[[i]],file);
        writeChar("}",nchars=1,eos=NULL,con=file)
      }else if(na[i]=="de"){
        stopifnot(class(out[[i]])=="character")
        xx <- past("de:{",out[[i]],"}",sep="")
        writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      }else{
        xx <- paste(na[i],":{",sep="")
        writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
        writenode(out[[i]],file);
        writeChar("}",nchars=1,eos=NULL,con=file)
      }
    }
  }
  writearray <- function(out,file){
    stopifnot(class(out)[1] %in%
              c("numeric","integer","integer64","array","matrix"));
    if(class(out)[1] %in% c("numeric","integer","integer64")){
      stopifnot(length(out)>0);
      if(class(out)[1]=="numeric"){
        x <- "r"
      }else{
        x <- "i"
      }
      if(class(out)[1]=="integer64"){
        out <- as.integer(out);
      }
      xx <- paste("ty:{",x,"}",sep="")
      writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      xx <- "bi:{";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      writeBin(64L,con=file,size=8)
      xx <- "}";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      xx <- "nd:{";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      writeBin(1L,con=file,size=8)
      xx <- "}";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      xx <- "di:{";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      writeBin(length(out),con=file,size=8)
      xx <- "}";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      xx <- paste("da:{",sep="");writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      writeBin(out,con=file,size=8)
      xx <- "}";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
    }else if(class(out)[1] %in% c("array","matrix")){
      stopifnot(length(dim(out))==2)
      stopifnot(sum(dim(out))>1)
      stopifnot(class(out[1,1])%in% c("integer","numeric","integer64"))
      if(class(out[1,1])=="numeric"){
        x <- "r";y=as.numeric(out)
      }else{
        x <- "s";y=as.integer(out)
      }
      xx <- paste("ty:{",x,"}",sep="");writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      xx <- "bi:{";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      writeBin(64L,con=file,size=8)
      xx <- "}";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      xx <- "nd:{";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      writeBin(length(dim(out)),con=file,size=8)
      xx <- "}";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      xx <- "di:{";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      writeBin(dim(out),con=file,size=8)
      xx <- "}";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      xx <- paste("da:{",sep="");writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
      writeBin(y,con=file,size=8)
      xx <- "}";writeChar(xx,nchars=nchar(xx),eos=NULL,con=file)
    }
  }
  unlink(file);
  ff <- file(description=file,open="wb")
  writeChar("{",nchars=1,eos=NULL,con=ff)
  writenode(out,ff);
  writeChar("}",nchars=1,eos=NULL,con=ff)
  close(ff)
}
readjarraybinary <- function(file){
  tryCatch(
  {
    readnode <- function(file,nodename){
      tryCatch(
      {
        k <- 1;i <- 0;nn <- c()
        out <- list()
        while(k!=0){
          x <- readChar(file,1L,useBytes=TRUE)
          if(x==":"){
            if(length(nn)==0){stop("error. found \":\" without preceeding character sequence.")}
            y <- readChar(file,1L,useBytes=TRUE)
            if(y!="{"){stop(paste("cannot detect sequence \":{\" after name ",file))}
            na <- paste(nn,collapse="");nn <- c()
            if(na=="de"){
              de <- c()
              kk <- 1
              while(kk>0){
                z <- readChar(file,1L,useBytes=TRUE)
                if(z=="}") break;
                de <- c(des,z)
              }
              i <- i+1;out[[i]] <- paste(de,collapse="");names(out)[i] <- "de"
            }else if(na=="array"){
              i <- i+1;out[[i]] <- readarray(file);
              names(out)[i] <- na;
              y <- readChar(file,1L,useBytes=TRUE)
              if(y!="}"){stop(paste("error. expecting \"}\", found \"",y,"\".",sep=""))}
            }else{
              i <- i+1;out[[i]] <- readnode(file,na);names(out)[i] <- na;
            }
          }else if(x=="}"){
            return(out);
          }else{
            nn <- c(nn,x)
          }
        }
      },error=function(c){
        c$message <- paste("error while reading node \"",nodename,"\".",c$message,sep="");stop(c);
      })
    }
    readarray <- function(file){
      tryCatch(
      {
        x <- readChar(file,4)
        if(x!="ty:{"){stop(paste("error. expecting \"ty:{\", found \"",x,"\".",sep=""))}
        type <- readChar(file,1L,useBytes=TRUE)
        if(!(type %in% c("i","r","c"))){
          stop(paste("error. expecting \"i\",\"r\" or \"c\", found \"",x,"\".",sep=""))
        }
        x <- readChar(file,1L,useBytes=TRUE)
        if(x!="}"){stop(paste("error. expecting \"}\", found \"",x,"\".",sep=""))}
        x <- readChar(file,4)
        if(x!="bi:{"){stop(paste("error. expecting \"bi:{\", found \"",x,"\".",sep=""))}
        bit <- readBin(file,"integer",1L,size=8)
        if(!(bit %in% as.integer(c(32,64)))){
          stop(paste("error. bit must be 32 or 64. found \"",bit,"\".",sep=""))
        }
        x <- readChar(file,1L,useBytes=TRUE)
        if(x!="}"){stop(paste("error. expecting \"}\", found \"",x,"\".",sep=""))}
        x <- readChar(file,4)
        if(x!="nd:{"){stop(paste("error. expecting \"nd:{\", found \"",x,"\".",sep=""))}
        nd <- readBin(file,"integer",1L,size=8)
        x <- readChar(file,1L,useBytes=TRUE)
        if(x!="}"){stop(paste("error. expecting \"}\", found \"",x,"\".",sep=""))}
        x <- readChar(file,4)
        if(x!="di:{"){stop(paste("error. expecting \"di:{\", found \"",x,"\".",sep=""))}
        di <- readBin(file,"integer",nd,size=8)
        x <- readChar(file,1L,useBytes=TRUE)
        if(x!="}"){stop(paste("error. expecting \"}\", found \"",x,"\".",sep=""))}
        x <- readChar(file,4)
        if(x!="da:{"){stop(paste("error. expecting \"da:{\", found \"",x,"\".",sep=""))}
        if(type=="i"){
          xx <- "integer";
        }else if(type=="r"){
          xx <- "numeric"
        }else{
          stop("unsupported type");
        }
        out  <- array(data=readBin(con=file,
                              what=xx,
                              n=as.integer(prod(di)),
                              size=as.integer(bit/8)),
                      dim=di);
        x <- readChar(file,1L,useBytes=TRUE)
        if(x!="}"){stop(paste("error. expecting \"}\", found \"",x,"\".",sep=""))}
        return(out);
      },error=function(c){
        c$message <- paste("error while reading array. ",c$message,sep="");stop(c);
      })
    }
    ff <- file(description=file,open="rb")
    x <- readChar(ff,1L,useBytes=TRUE)
    if(x!="{"){stop("first byte must be \"{\"")}
    out <- readnode(ff,"root");
    close(ff)
    return(out);
  },error=function(c){
    close(ff)
    c$message <- paste("error when reading file \"",file,"\".",c$message,sep="");stop(c);
  })
}
