/* A separate generated destination and save namespace, selected before loading.
 * Never modify the scene's shared MoonSettings asset or a running Moon world. */
#include "../../runtime/native/gameplay.h"
#include "api.h"
int _fltused=0;
static struct {
 PTR component,alive,resources,focused,typing,frame,go_ctor,go_add,go_tr,parent,destroy,active;
 PTR anchor_min,anchor_max,pivot,anchored,size,canvas_mode,canvas_order,text,font,font_size,color,raycast,alignment,image_color;
 PTR folder,dirname,combine,mkdir,instantiate,in_menu,loading,joined,ending_playing,prefs_get,prefs_set,prefs_save,refresh;
 PTR settings,settings_seed,override_root;
 void (*set_field)(PTR,PTR,void*);
 DRoot ui,label,moon_root,mars_root,menu,planet_world,planet_settings;DWORD thread;int selected,initialized,unlocked,bypass,last_frame,seed,direct;
 short (*key)(int);int held_m,held_n;
 PTR log;char config[1024];
} m;
static PTR mc(PTR obj,const char *ns,const char *name,int *ok){void *a[]={dtype(ns,name)};return a[0]?call(m.component,obj,a,ok):0;}
#include "ui.h"
#define UNLOCK_KEY "MoonLab.MarsPlanet.MoonCompleted.v1"
static void event(const char *value){if(!m.log)return;char b[1400],*p=cat(b,"{\"event\":\"");p=cat(p,value);p=cat(p,"\"}\n");DWORD n=0;WriteFile(m.log,b,(DWORD)(p-b),&n,0);}
static int token(PTR method_id,DWORD expected){return method_id&&method_token(method_id)==expected;}
static PTR mm(const char *ns,const char *type,const char *name,const char *sig){return dmethod(ns,type,name,sig);}
static void config(PTR game){
 typedef DWORD(__stdcall *FullPath)(const char*,DWORD,char*,char**);
 typedef unsigned(__stdcall *ReadInt)(const char*,const char*,int,const char*);
 PTR kernel=GetModuleHandleA("kernel32.dll");FullPath full=(FullPath)GetProcAddress(kernel,"GetFullPathNameA");ReadInt read=(ReadInt)GetProcAddress(kernel,"GetPrivateProfileIntA");
 char path[1024];DWORD n=GetEnvironmentVariableA("MOONLAB_MARS_PLANET_SETTINGS",path,sizeof(path));
 if(!n||n>=sizeof(path))cat(path,"MoonLab\\mods\\mars-planet\\settings.ini");
 if(!full||!full(path,sizeof(m.config),m.config,0))cat(m.config,path);
 m.seed=read?(int)read("mars-planet","seed",20261109,m.config):20261109;
 m.direct=read?(int)read("mars-planet","direct_menu_selection",1,m.config):1;
 (void)game;
}
static int roots(void){
 if(m.initialized)return 1;int ok=1;m.bypass=1;PTR base=call(m.folder,0,0,&ok);m.bypass=0;if(!ok||!base)return 0;
 m.moon_root=dhold(base);char suffix[96],*p=cat(suffix,"MoonLab.Planets/Mars/v1/seed-");p=num(p,(unsigned)m.seed);(void)p;
 /* Vanilla Migrate treats unknown MoonSessions children as a legacy world
  * and moves them into slot-1 on the next boot. Keep planets beside that root. */
 void *parent_args[]={dget(m.moon_root)};PTR parent=call(m.dirname,0,parent_args,&ok);if(!ok||!parent)return 0;
 void *a[]={parent,da.string(suffix)};PTR mars=call(m.combine,0,a,&ok);if(!ok||!mars)return 0;m.mars_root=dhold(mars);
 void *b[]={dget(m.mars_root)};call(m.mkdir,0,b,&ok);if(!ok)return 0;
 int zero=0;void *c[]={da.string(UNLOCK_KEY),&zero};PTR v=call(m.prefs_get,0,c,&ok);m.unlocked=v&&*(int*)unbox(v)>0;
 m.initialized=ok;event(ok?"save-namespaces-ready":"save-namespaces-failed");return ok;
}
static void unlock(void){
 if(m.selected||m.unlocked)return;int ok=1;
 if(!boolean(m.ending_playing,0,&ok)||!ok)return;
 int yes=1;void *a[]={da.string(UNLOCK_KEY),&yes};call(m.prefs_set,0,a,&ok);call(m.prefs_save,0,0,&ok);
 if(ok){m.unlocked=1;event("moon-ending-unlocked-mars");}
}
static int poll(int key,int *held){int down=(m.key(key)&0x8000)!=0;int pressed=down&&!*held;*held=down;return pressed;}
static PTR backup_root(int slot,int *ok){
 if(slot<0||!roots()){*ok=0;return 0;}
 char suffix[64];num(cat(suffix,"Backups/slot-"),(unsigned)slot);
 void *a[]={dget(m.mars_root),da.string(suffix)};return call(m.combine,0,a,ok);
}
static void menu_tick(PTR owner){
 int ok=1;if(!roots())return;
 if(!boolean(m.in_menu,0,&ok)||boolean(m.loading,0,&ok)||!ok)return;
 if(owner!=dget(m.menu)){destroy_root(&m.ui);ddrop(&m.label);ddrop(&m.menu);m.menu=dhold(owner);}
 int pm=poll('M',&m.held_m),pn=poll('N',&m.held_n);
 int ctrl=(m.key(0x11)&0x8000)!=0,alt=(m.key(0x12)&0x8000)!=0;
 int input=boolean(m.focused,0,&ok)&&!boolean(m.typing,0,&ok)&&!boolean(m.joined,0,&ok)&&ok;
 if(input&&ctrl&&alt&&(pm||pn)){
  int next=pm?1:0;if(!next||m.direct||m.unlocked){m.selected=next;call(m.refresh,owner,0,&ok);event(next?"destination-mars":"destination-moon");}
 }
 char text[512],*p=cat(text,m.selected?"Destination: MARS  /  separate save slots\n":"Destination: MOON  /  original save slots\n");
 p=cat(p,"Ctrl+Alt+M: Mars   Ctrl+Alt+N: Moon\n");p=cat(p,m.unlocked?"Moon completed: Mars unlocked. ":"Direct Mars start enabled. ");
 if(!m.direct&&!m.unlocked)p=cat(text,"Mars unlocks after the Moon ending.\nCtrl+Alt+N: Moon\n");
 cat(p,"Choose a slot and use the normal Play button.");show(text,1);
}
static void set_float(PTR settings,const char *name,float value){PTR f=get_field(object_class(settings),name);if(f)m.set_field(settings,f,&value);}
static float field_float(PTR settings,const char *name){float value=0;PTR f=get_field(object_class(settings),name);if(f)field_value(settings,f,&value);return value;}
static void scale(PTR settings,const char *name,float multiplier){set_float(settings,name,field_float(settings,name)*multiplier);}
static int apply_planet(PTR world){
 if(!m.selected)return 1;
 if(world==dget(m.planet_world))return 1;
 int ok=1;if(!roots())return 0;
 /* Guests must not fabricate an alternate local terrain/save namespace. Mars
  * co-op is deliberately unavailable until the destination handshake exists. */
 if(boolean(m.joined,0,&ok)||!ok){event("mars-coop-refused");return 0;}
 PTR source=0;field_value(world,m.settings,&source);if(!source)return 0;void *a[]={source};PTR clone=call(m.instantiate,0,a,&ok);if(!ok||!clone)return 0;
 DRoot guard=dhold(clone);m.set_field(clone,m.settings_seed,&m.seed);
 /* Preserve the authored base radius and spawn layout. Variation changes
  * collision-generating terrain, landforms, dust work and gravity. */
 scale(clone,"terrainHeight",1.65f);scale(clone,"mountainHeight",1.5f);scale(clone,"craterDensity",.4f);
 scale(clone,"landformScale",1.35f);scale(clone,"landscapeVariety",1.25f);scale(clone,"dustPatchSize",1.4f);
 set_float(clone,"gravity",3.71f);set_float(clone,"dustCoverage",.88f);scale(clone,"dustThicknessMax",1.25f);
 /* IL2CPP reference fields take the object itself, unlike scalar fields.
  * Passing &clone publishes a stack address that vanilla later dereferences. */
 m.set_field(world,m.settings,clone);
 PTR assigned=0;field_value(world,m.settings,&assigned);
 if(assigned!=clone){ddrop(&guard);event("mars-settings-assignment-failed");return 0;}
 /* Repeated Initialize on one world must not multiply the terrain settings. */
 if(!malive(dget(m.planet_world)))destroy_root(&m.planet_settings);
 else ddrop(&m.planet_settings);
 ddrop(&m.planet_world);m.planet_world=dhold(world);m.planet_settings=guard;
 /* The active slot resolves through the namespace hook, including the
  * vanilla summaries/backups, rather than redirecting only one save writer. */
 event("mars-generation-settings-applied");show("",0);return 1;
}
static int update(int id,Context *context){
 if(id==2){if(m.bypass||!m.selected)return 0;if(!roots()){context->rax=0;event("mars-folder-unavailable");return 1;}context->rax=(U64)dget(m.mars_root);return 1;}
 /* BackupFolder derives its path directly from persistentDataPath, bypassing
  * get_Folder. It needs its own boundary to avoid sharing Moon's backup slots. */
 if(id==4){if(!m.selected)return 0;int ok=1;context->rax=(U64)backup_root((int)context->rcx,&ok);if(!ok)event("mars-backup-folder-unavailable");return 1;}
 if(id==1){if(!apply_planet((PTR)context->rcx)){/* Fail before any world generation or save read. */event("mars-initialize-refused");return 1;}return 0;}
 if(!m.thread)m.thread=GetCurrentThreadId();if(m.thread!=GetCurrentThreadId())return 0;
 if(id==0)menu_tick((PTR)context->rcx);
 if(id==3){if(roots())unlock();if(m.unlocked&&!m.selected)show("Moon complete - Mars unlocked.\nReturn to the main menu, then Ctrl+Alt+M.\nMoon and Mars have separate persistent slots.",1);}
 return 0;
}
static int bind(PTR game){
#define B(F,N,T,M,S) m.F=mm(N,T,M,S);if(!m.F)return 0
 B(component,"UnityEngine","GameObject","GetComponent","System.Type");B(alive,"UnityEngine","Object","op_Implicit","UnityEngine.Object");B(resources,"UnityEngine","Resources","FindObjectsOfTypeAll","System.Type");B(focused,"UnityEngine","Application","get_isFocused","");B(typing,"MoonCleaners","MoonInput","get_Typing","");
 B(go_ctor,"UnityEngine","GameObject",".ctor","System.String");B(go_add,"UnityEngine","GameObject","AddComponent","System.Type");B(go_tr,"UnityEngine","GameObject","get_transform","");B(parent,"UnityEngine","Transform","SetParent","UnityEngine.Transform;System.Boolean");B(destroy,"UnityEngine","Object","Destroy","UnityEngine.Object");B(active,"UnityEngine","GameObject","SetActive","System.Boolean");
 B(anchor_min,"UnityEngine","RectTransform","set_anchorMin","UnityEngine.Vector2");B(anchor_max,"UnityEngine","RectTransform","set_anchorMax","UnityEngine.Vector2");B(pivot,"UnityEngine","RectTransform","set_pivot","UnityEngine.Vector2");B(anchored,"UnityEngine","RectTransform","set_anchoredPosition","UnityEngine.Vector2");B(size,"UnityEngine","RectTransform","set_sizeDelta","UnityEngine.Vector2");B(canvas_mode,"UnityEngine","Canvas","set_renderMode","UnityEngine.RenderMode");B(canvas_order,"UnityEngine","Canvas","set_sortingOrder","System.Int32");
 B(text,"UnityEngine.UI","Text","set_text","System.String");B(font,"UnityEngine.UI","Text","set_font","UnityEngine.Font");B(font_size,"UnityEngine.UI","Text","set_fontSize","System.Int32");B(color,"UnityEngine.UI","Graphic","set_color","UnityEngine.Color");B(raycast,"UnityEngine.UI","Graphic","set_raycastTarget","System.Boolean");B(alignment,"UnityEngine.UI","Text","set_alignment","UnityEngine.TextAnchor");B(image_color,"UnityEngine.UI","Graphic","set_color","UnityEngine.Color");
 B(folder,"MoonCleaners","MoonSaveSlots","get_Folder","");B(dirname,"System.IO","Path","GetDirectoryName","System.String");B(combine,"System.IO","Path","Combine","System.String;System.String");B(mkdir,"System.IO","Directory","CreateDirectory","System.String");B(instantiate,"UnityEngine","Object","Instantiate","UnityEngine.Object");
 B(in_menu,"MoonCleaners","MoonSession","get_InMenu","");B(loading,"MoonCleaners","MoonSession","get_Loading","");B(joined,"MoonCleaners","MoonCrew","get_Joined","");B(ending_playing,"MoonCleaners","MoonEnding","get_Playing","");B(refresh,"MoonCleaners","MoonMainMenu","RefreshSlots","");
 B(prefs_get,"UnityEngine","PlayerPrefs","GetInt","System.String;System.Int32");B(prefs_set,"UnityEngine","PlayerPrefs","SetInt","System.String;System.Int32");B(prefs_save,"UnityEngine","PlayerPrefs","Save","");
#undef B
 m.settings=get_field(find_class("MoonCleaners","MoonWorld"),"settings");m.settings_seed=get_field(find_class("MoonCleaners","MoonSettings"),"seed");
 m.set_field=(void(*)(PTR,PTR,void*))GetProcAddress(game,"il2cpp_field_set_value");
 PTR user=GetModuleHandleA("user32.dll");if(!user)user=LoadLibraryA("user32.dll");m.key=(short(*)(int))GetProcAddress(user,"GetAsyncKeyState");
 return m.settings&&m.settings_seed&&m.set_field&&m.key&&token(m.folder,100670115)&&token(m.refresh,100668110)&&token(m.ending_playing,100666801);
}
__declspec(dllexport) int MoonLab_ModStart(PTR domain,PTR game){
 char path[1024];DWORD n=GetEnvironmentVariableA("MOONLAB_EVIDENCE",path,900);if(n&&n<900){cat(path+n,"\\mars-planet.jsonl");m.log=CreateFileA(path,0x40000000,1,0,1,0x80,0);if(m.log==(PTR)(U64)-1)m.log=0;}
 if(!bind_runtime(domain,game)||!dapi(game)||!bind(game))return 0;config(game);event("configuration-resolved");
 if(m.log){char b[1400],*p=cat(b,"{\"settings_path\":\"");for(const char *s=m.config;*s;s++){if(*s=='\\'||*s=='"')*p++='\\';*p++=*s;}p=cat(p,"\",\"seed\":");p=num(p,(unsigned)m.seed);p=cat(p,"}\n");DWORD wrote=0;WriteFile(m.log,b,(DWORD)(p-b),&wrote,0);}
 before_hook=update;
 if(!prepare_hook(mm("MoonCleaners","MoonMainMenu","Update",""),100668088,0,game)||!prepare_hook(mm("MoonCleaners","MoonWorld","Initialize",""),100664654,1,game)||!prepare_hook(m.folder,100670115,2,game)||!prepare_hook(mm("MoonCleaners","MoonEnding","Update",""),100666808,3,game)||!prepare_hook(mm("MoonCleaners","MoonSaveSlots","BackupFolder","System.Int32"),100670117,4,game)||!install_hooks())return 0;
 (void)integer;(void)dzero;(void)dnumber;(void)dvector;event("ready-testing-deferred");return 1;
}
