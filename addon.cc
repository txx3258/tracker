#include <node.h>
#define N 134217728;

using namespace v8;

unsigned int JSHash(char* str,unsigned int len);
unsigned int RSHash(char* str,unsigned int len);
unsigned int PJWHash(char* str,unsigned int len);
unsigned int ELFHash(char* str,unsigned int len);
unsigned int BKDRHash(char* str,unsigned int len);
unsigned int SDBHash(char* str,unsigned int len);
unsigned int DEKHash(char* str,unsigned int len);
unsigned int BPHash(char* str,unsigned int len);
unsigned int FNVHash(char* str,unsigned int len);
unsigned int APHash(char* str,unsigned int len);

void hashkeys(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  String::utf8Value str(args[0]);
  char* key = *str;
  int len=args[1]->NumberValue();
  
  Local<Array> nodes = Number::New(isolate, value);

  Local<Number> no1=Number::New(isolate, JSHash(key,len));
	Local<Number> no4=Number::New(isolate, RSHash(key,len));
	Local<Number> no3=Number::New(isolate, APHash(key,len));
	Local<Number> no2=Number::New(isolate, ELFHash(key,len));

  nodes->Set(0, no1);
  nodes->Set(1, no2);
  nodes->Set(2, no3);
  nodes->Set(3, no4);

  args.GetReturnValue().Set(nodes);
}

void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "hashkeys", hashkeys);
}

unsigned int RSHash(char* str, unsigned int len){
   unsigned int b    = 378551;  
   unsigned int a    = 63689;  
   unsigned int hash = 0;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = hash * a + (*str);  
      a    = a * b;  
   }  
  
   return hash%N;  
}  
/* End Of RS Hash Function */  

unsigned int JSHash(char* str, unsigned int len){
   unsigned int hash = 1315423911;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash ^= ((hash << 5) + (*str) + (hash >> 2));  
   }  
  
   return hash%N;  
}  
/* End Of JS Hash Function */  

unsigned int PJWHash(char* str, unsigned int len){
   const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);  
   const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);  
   const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);  
   const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);  
   unsigned int hash              = 0;  
   unsigned int test              = 0;  
   unsigned int i                 = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = (hash << OneEighth) + (*str);  
  
      if((test = hash & HighBits)  != 0){
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));  
      }  
   }  
  
   return hash%N;  
}  
/* End Of  P. J. Weinberger Hash Function */  
  
unsigned int ELFHash(char* str, unsigned int len){
   unsigned int hash = 0;  
   unsigned int x    = 0;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = (hash << 4) + (*str);  
      if((x = hash & 0xF0000000L) != 0)  
      {  
         hash ^= (x >> 24);  
      }  
      hash &= ~x;  
   }  
  
   return hash%N;  
}  
/* End Of ELF Hash Function */  
  
unsigned int BKDRHash(char* str, unsigned int len){
   unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */  
   unsigned int hash = 0;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = (hash * seed) + (*str);  
   }  
  
   return hash%N;  
}  
/* End Of BKDR Hash Function */  
  
unsigned int SDBMHash(char* str, unsigned int len){
   unsigned int hash = 0;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = (*str) + (hash << 6) + (hash << 16) - hash;  
   }  
  
   return hash%N;  
}  
/* End Of SDBM Hash Function */  
  
unsigned int DJBHash(char* str, unsigned int len){
   unsigned int hash = 5381;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = ((hash << 5) + hash) + (*str);  
   }  
  
   return hash%N;  
}  
/* End Of DJB Hash Function */  
  
unsigned int DEKHash(char* str, unsigned int len){
   unsigned int hash = len;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);  
   }  
   return hash%N;  
}  
/* End Of DEK Hash Function */  
  
unsigned int BPHash(char* str, unsigned int len){
   unsigned int hash = 0;  
   unsigned int i    = 0;  
   for(i = 0; i < len; str++, i++){
      hash = hash << 7 ^ (*str);  
   }  
  
   return hash%N;  
}  
/* End Of BP Hash Function */  
  
unsigned int FNVHash(char* str, unsigned int len){
   const unsigned int fnv_prime = 0x811C9DC5;  
   unsigned int hash      = 0;  
   unsigned int i         = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash *= fnv_prime;  
      hash ^= (*str);  
   }  
  
   return hash%N;  
}  
/* End Of FNV Hash Function */  
  
unsigned int APHash(char* str, unsigned int len){
   unsigned int hash = 0xAAAAAAAA;  
   unsigned int i    = 0;  
  
   for(i = 0; i < len; str++, i++){
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :  
                               (~((hash << 11) + ((*str) ^ (hash >> 5))));  
   }  
  
   return hash%N;  
}  
/* End Of AP Hash Function */  
NODE_MODULE(addon, Init)
