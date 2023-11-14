int a=0,b=0;
typedef char string[80];

struct field                //structure to store column details
{ char fnm[20];
  int dt;
};


class t_struct              //class to store table structure
{ public:

  field f[10];
  int noc;

  t_struct()                //default constructor
  {
    noc=0;
  }

  t_struct(int n)           //parametrized constructor
  {
    noc=n;
  }

  ~t_struct()               //destructor
  { ;
  }
};

class data                  //class to store table data
{ public:

  int i[10],ni,ns;
  char s[20][10];

  data();                   //constructor

  ~data()                   //destructor
  { ;
  }

  void disp();

};
  void data::disp()
  { for(int j=0;j<ni;j++)
     cout<<i[j]<<'\t';
    for(j=0;j<ns;j++)
     cout<<s[j]<<'\t';
    cout<<endl;
  }

  data::data()
  { for(int j=0;j<10;j++)
    { i[j]=0;
      strcpy(s[j],"null");
    }
    ni=ns=0;
  }

char *enter_query()
{ char *str,*query;
  str=new string;
  query=new char[200];
  do { gets(str);
       strcat(query,str);
     } while(str[strlen(str)-1]!=';');
  return query;
}

int extract_cmd(char str[])
{ int i=0,j=0;
  char *s=NULL;
  s=new char[20];
  while(str[i++]!='#');
  while(str[i]!=' ')
   s[j++]=str[i++];
  s[j]='\0';
  if(!strcmpi(s,"create"))
   return 1;
  else if(!strcmpi(s,"insert"))
   return 2;
  else if(!strcmpi(s,"search"))
   return 3;
  else if(!strcmpi(s,"delete"))
   return 4;
  else if(!strcmpi(s,"update"))
   return 5;
  else if(!strcmpi(s,"display"))
   return 6;
  else if(!strcmpi(s,"structure"))
   return 7;
  else if(!strcmpi(s,"exit"))
   return 8;
  else
   return 0;
}

char *extract_tnm(char str[])
{ int i=0,j=0;
  char s[20];
  s[0]='\0';
  while(str[i++]!='\\');
  while(str[i]!=' ')
   s[j++]=str[i++];
  s[j]='\0';
  return s;
}

int extract_dtype(char str[],int &i)
{ char *s;
  int j=0;
  s=new char[20];
  while(str[i]!=')')
   s[j++]=str[i++];
  s[j]='\0';
  if(!strcmpi(s,"int"))
  { delete s;
    return 1;
  }
  else if(!strcmpi(s,"str"))
  { delete s;
    return 2;
  }
  else
  { delete s;
    return 0;
  }
}

field extract_par(char str[],int &i)
{ char *st;
  int j=0;
  field f;
  st=new char[20];
  while(str[i++]!='(');
  while(str[i]!=' ')
   st[j++]=str[i++];
  i++;
  st[j]='\0';
  strcpy(f.fnm,st);
  f.dt=extract_dtype(str,i);
  delete st;
  return f;
}

int extract_val_i(char str[],int &k)
{ int n=0,j=0,nn,i=0;
  char no[5];
  while(str[k++]!='$');
  while(str[k]!=' ')
   no[j++]=str[k++];
  no[j]='\0';
  while(no[i]!='\0')
  { nn=no[i++]-48;
    n=n*10+nn;
  }
  return n;
}

void extract_val_s(char str[],int &i,char a[])
{ int j=0;
  while(str[i++]!='$');
  while(str[i]!=' ')
   a[j++]=str[i++];
  a[j]='\0';
}

int extract_spar_i(char str[],int &k)
{ int n=0,j=0,nn=0,i=0;
  char no[5];
  while(str[k++]!='=');
  while(str[k]!=' ')
   no[j++]=str[k++];
  no[j]='\0';
  while(no[i]!='\0')
  { nn=no[i++]-48;
    n=n*10+nn;
  }
  return n;
}

void extract_spar_s(char str[],int &i,char a[])
{ int j=0;
  while(str[i++]!='=');
  while(str[i]!=' ')
   a[j++]=str[i++];
  a[j]='\0';
}

char *extract_s_fnm(char str[],char s[])
{ int i=0,j=0;
  s[0]='\0';
  while(str[i++]!='@');
  while(str[i]!=' ')
   s[j++]=str[i++];
  s[j]='\0';
  return s;
}



int extract_noc(char str[])
{ int n=0,i=0,j=0,nn;
  char no[5];
  while(str[i++]!='*');
  while(str[i]!=' ')
   no[j++]=str[i++];
  no[j]='\0';
  i=0;
  while(no[i]!='\0')
  { nn=no[i++]-48;
    n=n*10+nn;
  }
  if(nn>10||nn<1)
   return 0;
  else
   return n;
}

void create(char tnm[],char str[])
{ ofstream fout;
  int n;
  n = extract_noc(str);
  t_struct t(n);
  int i=0,j=0,k=0;
  while(k++<n)
  { t.f[j++]=extract_par(str,i);
  }
  if(tnm)
   strcat(tnm,".dat");

  fout.open(tnm,ios::binary|ios::noreplace);
  if(!fout)
   cout<<"Error code 103\n";
  else
  { fout.write((char *)&t,sizeof(t));
    cout<<"Query OK...\n";
  }
  fout.close();
}

void insert(char tnm[],char str[])
{ ifstream fin;
  ofstream fout;
  data d;
  t_struct t;
  int c=0,a=0,b=0,i=0;
  if(tnm)
   strcat(tnm,".dat");
  fin.open(tnm,ios::binary);
  if(!fin)
   cout<<"Error code 103\n";
  else
  { fin.seekg(0);
    fin.read((char *)&t,sizeof(t));
  }
  fin.close();
  while(t.f[c].dt==1 && c<t.noc)
  { d.i[a++]=extract_val_i(str,i);
    d.ni++;
    c++;
  }
  while(c<t.noc)
  { extract_val_s(str,i,d.s[b++]);
    d.ns++;
    c++;
  }

  fout.open(tnm,ios::binary|ios::app|ios::nocreate);
  if(!fout)
   cout<<"Error code 103\n";
  else
  {
    fout.write((char *)&d,sizeof(d));
    cout<<"Query OK...\n";
  }
  fout.close();
}

void display(char tnm[])
{ ifstream fin;
  data d;
  int i=0;
  t_struct t;
  if(tnm)
   strcat(tnm,".dat");
  fin.open(tnm,ios::binary);
  if(!fin)
   cout<<"Error code 103\n";
  else
  { fin.seekg(0);
    fin.read((char *)&t,sizeof(t));
    while(i<t.noc)
    { cout<<t.f[i++].fnm<<'\t';
    }
    cout<<'\n';
    while(fin.read((char *)&d,sizeof(d)))
    { d.disp();
    }
  }
  fin.close();
}

void display_struct(char tnm[])
{ ifstream fin;
  int i=0;
  t_struct t;
  if(tnm)
   strcat(tnm,".dat");
  fin.open(tnm,ios::binary);
  if(!fin)
   cout<<"Error code 103\n";
  else
  { fin.seekg(0);
    fin.read((char *)&t,sizeof(t));
    while(i<t.noc)
    { cout<<t.f[i-1].fnm<<'\t'<<t.f[i++].dt<<'\n';
    }
  }
  fin.close();
}

void search(char tnm[],char str[])
{ int i=0,n=0,m=0,r=0,j=0;
  char s[20],fnm[20];
  ifstream fin;
  t_struct t;
  data d;
  extract_s_fnm(str,fnm);
  if(tnm)
   strcat(tnm,".dat");

  fin.open(tnm,ios::binary);
  if(!fin)
   cout<<"Error code 103\n";
  else
  { fin.seekg(0);
    fin.read((char *)&t,sizeof(t));
    while(i<t.noc)
    { if(!strcmpi(t.f[i++].fnm,fnm))
       break;
    }
    i--;
    if(t.f[i].dt==1)
    { n=extract_spar_i(str,m);
    }
    else
    { extract_spar_s(str,m,s);
    }
    while(j<t.noc)
    { cout<<t.f[j++].fnm<<'\t';
    }
    cout<<'\n';
    while(fin.read((char *)&d,sizeof(d)))
    { if(t.f[i].dt==1)
      { if(n==d.i[i])
	{ d.disp();
	  r++;
	}
      }
      else
      { if(!strcmpi(s,d.s[i-d.ni]))
	{ d.disp();
	  r++;
	}
      }
    }
  }
  cout<<r<<" records found...\n";
  fin.close();
  delete fnm;
}

void delet(char tnm[],char str[])
{ int i=0,n=0,m=0,j=0;
  char s[20],fnm[20];
  ifstream fin;
  ofstream fout;
  t_struct t;
  data d;
  extract_s_fnm(str,fnm);
  if(tnm)
   strcat(tnm,".dat");
  fout.open("temp.dat",ios::binary);
  fin.open(tnm,ios::binary);
  if(!fin||!fout)
   cout<<"Error code 103\n";
  else
  { fin.seekg(0);
    fin.read((char *)&t,sizeof(t));
    while(i<t.noc)
    { if(!strcmpi(t.f[i++].fnm,fnm))
       break;
    }
    i--;
    if(t.f[i].dt==1)
    { n=extract_spar_i(str,m);
    }
    else
    { extract_spar_s(str,m,s);
    }
    cout<<"The following record(s) has been deleted: \n";
    while(j<t.noc)
    { cout<<t.f[j++].fnm<<'\t';
    }
    cout<<'\n';
    while(fin.read((char *)&d,sizeof(d)))
    { if(t.f[i].dt==1)
      { if(n==d.i[i])
       { d.disp();
       }
	else
	 fout.write((char *)&d,sizeof(d));
      }
      else
      { if(!strcmpi(s,d.s[i-d.ni]))
	{ d.disp();
	}
	else
	 fout.write((char *)&d,sizeof(d));
      }
    }
  }
  fin.close();
  fout.close();
  remove(tnm);
  rename("temp.dat",tnm);
  delete fnm;
}

void update(char tnm[],char str[])
{ int i=0,n=0,j=0,m=0,c=0,k=0,l=0;
  char s[20],fnm[20];
  fstream finout;
  t_struct t;
  data d;
  extract_s_fnm(str,fnm);
  if(tnm)
   strcat(tnm,".dat");

  finout.open(tnm,ios::binary|ios::in|ios::ate|ios::out);
  if(!finout)
   cout<<"Error code 103\n";
  else
  { finout.seekg(0);
    finout.read((char *)&t,sizeof(t));
    while(i<t.noc)
    { if(!strcmpi(t.f[i++].fnm,fnm))
       break;
    }
    i--;
    if(t.f[i].dt==1)
    { n=extract_spar_i(str,m);
    }
    else
    { extract_spar_s(str,m,s);
    }
    while(j<t.noc)
    while(finout.read((char *)&d,sizeof(d)))
    { if(t.f[i].dt==1)
      { if(n==d.i[i])
	{ cout<<"Record to be updated:\n";
	  d.disp();
	  while(c<t.noc)
	  { cout<<t.f[c].fnm<<": ";
	    if(t.f[c++].dt==1)
	     cin>>d.i[k++];
	    else
	     gets(d.s[l++]);
	  }
	  finout.seekg(finout.tellg()-sizeof(d));
	  finout.write((char *)&d,sizeof(d));

	  break;
	}

	else
	{ if(!strcmpi(s,d.s[i-d.ni]))
	  { while(c<t.noc)
	    { cout<<t.f[c].fnm<<": ";
	      if(t.f[c++].dt==1)
	       cin>>d.i[k++];
	      else
	       gets(d.s[l++]);
	    }
	    finout.seekg(finout.tellg()-sizeof(d));
	    finout.write((char *)&d,sizeof(d));

	    break;
	  }
	}
      }
    }
    cout<<"Query OK...\n";
  }

  finout.close();
  delete fnm;
}

void start()
{ clrscr();
  char str[400],tname[20],ch;
  int keyword=0;

  do{ str[0]='\0';
      tname[0]='\0';
      cout<<":\\";
      strcpy(str,enter_query());
      str[strlen(str)-1]='\0';
      keyword=extract_cmd(str);
      strcpy(tname,extract_tnm(str));
      if(tname)
      { switch(keyword)
	{ case 0: cout<<"Error code 105\n";
		  break;

	  case 1: create(tname,str);
		  break;

	  case 2: insert(tname,str);
		  break;

	  case 3: search(tname,str);
		  break;

	  case 4: delet(tname,str);
		  break;

	  case 5: update(tname,str);
		  break;

	  case 6: display(tname);
		  break;

	  case 7: display_struct(tname);
		  break;


	}
      }
    }while(keyword!=8);
  cout<<"Exiting...";
  delay(1000);
}
