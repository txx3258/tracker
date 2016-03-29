#include <node.h>
#define N 134217728;

using namespace v8;

int JSHash(char* str,int len);
int RSHash(char* str,int len);
int PJWHash(char* str,int len);
int ELFHash(char* str,int len);
int BKDRHash(char* str,int len);
int SDBMHash(char* str,int len);
int DEKHash(char* str,int len);
int BPHash(char* str,int len);
int FNVHash(char* str,int len);
int APHash(char* str,int len);

void hashkeys(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  String::Utf8Value str(args[0]);
  int len=args[1]->NumberValue();
  char* key=*str;
  
  Local<Array> nodes = Array::New(isolate,4);
  Local<Number> no1=Number::New(isolate, JSHash(key,len));
  Local<Number> no3=Number::New(isolate, APHash(key,len));
  Local<Number> no2=Number::New(isolate, ELFHash(key,len));
  Local<Number> no4=Number::New(isolate, RSHash(key,len));
  
  nodes->Set(0, no1);
  nodes->Set(1, no2);
  nodes->Set(2, no3);
  nodes->Set(3, no4);

  args.GetReturnValue().Set(nodes);
}

void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "hashkeys", hashkeys);
}

int RSHash(char* str, int len){
   int b    = 378551;  
   int a    = 63689;  
   int hash = 0;  
   int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = hash * a + (*str);  
      a    = a * b;  
   }  
  
   return (hash&0x7FFFFFFF)%N;  
}  
/* End Of RS Hash Function */  

int JSHash(char* str, int len){
   int hash = 1315423911;  
   int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash ^= ((hash << 5) + (*str) + (hash >> 2));  
   }  
  
   return (hash&0x7FFFFFFF)%N;  
}  
/* End Of JS Hash Function */  

int PJWHash(char* str, int len){
   const int BitsInUnsignedInt = (int)(sizeof(int) * 8);  
   const int ThreeQuarters     = (int)((BitsInUnsignedInt  * 3) / 4);  
   const int OneEighth         = (int)(BitsInUnsignedInt / 8);  
   const int HighBits          = (int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);  
   int hash              = 0;  
   int test              = 0;  
   int i                 = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = (hash << OneEighth) + (*str);  
  
      if((test = hash & HighBits)  != 0){
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));  
      }  
   }  
  
   return hash%N;  
}  
/* End Of  P. J. Weinberger Hash Function */  
  
int ELFHash(char* str, int len){
   int hash = 0;  
   int x    = 0;  
   int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = (hash << 4) + (*str);  
      if((x = hash & 0xF0000000L) != 0)  
      {  
         hash ^= (x >> 24);  
      	 hash &= ~x;  
      }  
   }  
  
   return (hash&0x7FFFFFFF)%N;  
}  
/* End Of ELF Hash Function */  
  
int BKDRHash(char* str, int len){
   int seed = 131; /* 31 131 1313 13131 131313 etc.. */  
   int hash = 0;  
   int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = (hash * seed) + (*str);  
   }  
  
   return (hash&0x7FFFFFFF)%N;  
}  
/* End Of BKDR Hash Function */  
  
int SDBMHash(char* str, int len){
   int hash = 0;  
   int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = (*str) + (hash << 6) + (hash << 16) - hash;  
   }  
  
   return (hash&0x7FFFFFFF)%N;  
}  
/* End Of SDBM Hash Function */  
  
int DJBHash(char* str, int len){
   int hash = 5381;  
   int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = ((hash << 5) + hash) + (*str);  
   }  
  
   return (hash&0x7FFFFFFF)%N;  
}  
/* End Of DJB Hash Function */  
  
int DEKHash(char* str, int len){
   int hash = len;  
   int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);  
   }  
   return (hash&0x7FFFFFFF)%N;  
}  
/* End Of DEK Hash Function */  
  
int BPHash(char* str, int len){
   int hash = 0;  
   int i    = 0;  
   for(i = 0; i < len; str++, i++){
      hash = hash << 7 ^ (*str);  
   }  
  
   return (hash&0x7FFFFFFF)%N;  
}  
/* End Of BP Hash Function */  
  
int FNVHash(char* str, int len){
   const int fnv_prime = 0x811C9DC5;  
   int hash      = 0;  
   int i         = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash *= fnv_prime;  
      hash ^= (*str);  
   }  
  
   return (hash&0x7FFFFFFF)%N;  
}  
/* End Of FNV Hash Function */  
  
int APHash(char* str, int len){
   int hash = 0xAAAAAAAA;  
   int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :  
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));  
   }  
  
   return (hash&0x7FFFFFFF)%N;  
}  
/* End Of AP Hash Function */  
NODE_MODULE(addon, Init)
