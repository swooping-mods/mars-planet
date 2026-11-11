typedef struct {float x,y;} V2;
static inline int malive(PTR obj){if(!obj)return 0;int ok=1;void *a[]={obj};PTR v=call(m.alive,0,a,&ok);return ok&&v&&*(unsigned char*)unbox(v);}

static inline void destroy_root(DRoot *root){PTR obj=dget(*root);if(malive(obj)){int ok=1;void *a[]={obj};call(m.destroy,0,a,&ok);}ddrop(root);}
static inline void mset(PTR method_id,PTR obj,void *value){int ok=1;void *a[]={value};call(method_id,obj,a,&ok);}
static inline PTR go(const char *name,PTR parent,int *ok){PTR o=da.new_object(find_class("UnityEngine","GameObject"));void *a[]={da.string(name)};call(m.go_ctor,o,a,ok);if(parent){PTR tr=call(m.go_tr,o,0,ok);unsigned char keep=0;void *b[]={parent,&keep};call(m.parent,tr,b,ok);}return o;}
static inline PTR add(PTR obj,const char *ns,const char *type,int *ok){void *a[]={dtype(ns,type)};return call(m.go_add,obj,a,ok);}
static inline void rect(PTR obj,float x,float y,float w,float h,int *ok){PTR r=mc(obj,"UnityEngine","RectTransform",ok);V2 one={1,1},pos={x,y},extent={w,h};mset(m.anchor_min,r,&one);mset(m.anchor_max,r,&one);mset(m.pivot,r,&one);mset(m.anchored,r,&pos);mset(m.size,r,&extent);}
static inline int ui(void){
 if(malive(dget(m.ui)))return 1;ddrop(&m.ui);ddrop(&m.label);int ok=1;
 PTR root=go("Mars destination",0,&ok);m.ui=dhold(root);PTR canvas=add(root,"UnityEngine","Canvas",&ok);int mode=0,order=252; mset(m.canvas_mode,canvas,&mode);mset(m.canvas_order,canvas,&order);
 PTR tr=call(m.go_tr,root,0,&ok),panel=go("Mars destination panel",tr,&ok);add(panel,"UnityEngine","RectTransform",&ok);PTR image=add(panel,"UnityEngine.UI","Image",&ok);DColor bg={.055f,.06f,.065f,.94f};mset(m.image_color,image,&bg);unsigned char no=0;mset(m.raycast,image,&no);rect(panel,-16,-16,500,126,&ok);
 PTR label=go("Mars destination text",call(m.go_tr,panel,0,&ok),&ok);add(label,"UnityEngine","RectTransform",&ok);PTR text=add(label,"UnityEngine.UI","Text",&ok);m.label=dhold(text);rect(label,-10,-8,480,110,&ok);
 void *a[]={dtype("UnityEngine","Font")};DRoot fonts=dhold(call(m.resources,0,a,&ok));PTR f=fonts&&da.array_length(dget(fonts))?*(PTR*)darray(dget(fonts),0):0;if(!f)ok=0;else {void *b[]={f};call(m.font,text,b,&ok);}ddrop(&fonts);
 int size=16,align=0;mset(m.font_size,text,&size);mset(m.alignment,text,&align);DColor fg={.92f,.93f,.94f,1};mset(m.color,text,&fg);mset(m.raycast,text,&no);
 return ok;
}
static inline void show(const char *text,int visible){if(!ui())return;unsigned char yes=(unsigned char)visible;mset(m.active,dget(m.ui),&yes);if(visible){int ok=1;void *a[]={da.string(text)};call(m.text,dget(m.label),a,&ok);}}
