#include "s3e.h"
#include "IwUtil.h"
#include "IwGx.h"
#include "IwGeomMat.h"

#include "game.h"

#include "muncher.h"
#include "shooter.h"

#define	MS_PER_FRAME (1000 / 10)

// Structure to track touches.
struct CTouch {
	int32 x;		// position
	int32 y;		// position
	bool active;	// whether touch is currently active
	int32 id;		// touch's unique identifier
    Unit* unit;
};

Game* game = NULL;

#define MAX_TOUCHES 10
CTouch touches[MAX_TOUCHES];

// find an active touch with the specified id, or allocate a free one from the list.
CTouch* GetTouch(int32 id) {
	CTouch* pInActive = NULL;
    
	for(uint32 i=0; i < MAX_TOUCHES; i++) {
		// if we've found a touch with the specified id, return its memory address.
		if(id == touches[i].id) return &touches[i];
		// if no touches are found with the id, pInActive will end up being set to
		// last touch in g_Touches.
		if(!touches[i].active) pInActive = &touches[i];
	}
    
	// return last inactive touch and assign its id to the specified id.
	if (pInActive) {
		pInActive->id = id;
		return pInActive;
	}
    
	// no more touches; give up.
	return NULL;
}


bool update() {    
	return true;
}

#define SQ(x) (x*x)
bool renderTouch(CTouch* touch) {
    if(!touch->unit)
        return false;
    
    int w = IwGxGetScreenWidth();
    int h = IwGxGetScreenHeight();
    
    int32 x = touch->x, y = h - touch->y;
    
    CIwFVec2 radii = game->getWorldRadius();
    
    int32 world_x = x + (radii.x - 20);
    int32 world_y = y - h/2;
    
    
    float theta = TO_RADIANS(game->getRotation());
    
    int32 model_x = world_x * cos(theta) - world_y * sin(theta);
    int32 model_y = world_x * sin(theta) + world_y * cos(theta);
    
    float dist_sq = SQ(model_x) + SQ(model_y);

    if(dist_sq > SQ(radii.y) || dist_sq < SQ(radii.x))
        return false;

    touch->unit->setPosition(model_x, model_y);
	game->addUnit(touch->unit);
        
    return true;
}


// assign activity and position info to the touch struct associated with an event
// for a multitouch click.
void MultiTouchButtonCB(s3ePointerTouchEvent* event) {
	CTouch* touch = GetTouch(event->m_TouchID);
	if (touch) {
		touch->active = event->m_Pressed != 0;
		touch->x = event->m_x;
		touch->y = event->m_y;
        if(touch->active && touch->x > IwGxGetScreenWidth() - 60){
            touch->unit = new Muncher(NULL, game, CIwFVec2(0,0));
        } else {
            renderTouch(touch); 
            touch->unit = NULL;
        }
	}
}

// assign position info to the touch struct associated with an event for
// multitouch motion.
void MultiTouchMotionCB(s3ePointerTouchMotionEvent* event) {
	CTouch* touch = GetTouch(event->m_TouchID);
	if (touch) {
		touch->x = event->m_x;
		touch->y = event->m_y;
	}
}

void doMain() {
    
    s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchButtonCB, NULL);
    s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)MultiTouchButtonCB, NULL);

	s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB, NULL);
    s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB, NULL);

    
    IwGetResManager()->LoadGroup("resource_groups/palate.group");
    CIwResGroup* palateGroup = IwGetResManager()->GetGroupNamed("Palate");
    CIwMaterial* mat = new CIwMaterial();
    mat->SetTexture((CIwTexture*)palateGroup->GetResNamed("palate", IW_GX_RESTYPE_TEXTURE));
    mat->SetModulateMode(CIwMaterial::MODULATE_NONE);
    mat->SetAlphaMode(CIwMaterial::ALPHA_DEFAULT);
        
	static CIwSVec2 xy(260, 0);
	static CIwSVec2 wh(60, 480);
	static CIwSVec2 uv(0, 0);
	static CIwSVec2 duv(1 << 11, 1 << 11);

    
	game = new Game(2);

    CTouch t;
    t.x = 40;
    t.y = 480 / 2;
    t.unit = new Muncher(NULL, game, CIwFVec2(0,0));
    renderTouch(&t);
    
//    CTouch t2;
//    t2.x = 150;
//    t2.y = 480 / 2 + 50;
//    t2.unit = new Muncher(NULL, game, CIwFVec2(0,0));
//    renderTouch(&t2);

	
	while (1) {
	
		s3eDeviceYield(0);
		s3eKeyboardUpdate();
		s3ePointerUpdate();
		
		if ((s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_DOWN)
				|| (s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_DOWN)       
				|| (s3eDeviceCheckQuitRequest())) {
			
		    break;
		}
        
        IwGxSetMaterial(mat);
        IwGxSetScreenSpaceSlot(-1);
        IwGxDrawRectScreenSpace(&xy, &wh, &uv, &duv);
		
        IwGxSetColClear(255, 255, 255, 255);
        IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
        
        game->tick();
		
		int64 start = s3eTimerGetMs();
        
        IwGxSwapBuffers();
		
		// Attempt frame rate
		while ((s3eTimerGetMs() - start) < MS_PER_FRAME)
		{
			int32 yield = (int32) (MS_PER_FRAME - (s3eTimerGetMs() - start));
			if (yield < 0) {
				break;
			}
				
			s3eDeviceYield(yield);
		}
	}
    
    s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchButtonCB);
    s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)MultiTouchButtonCB);

	s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB);
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB);
    
	delete game;
}

int main() {
	IwGxInit();
	IwResManagerInit();
 
	doMain();
	
	IwResManagerTerminate();
	IwGxTerminate();
		
	return 0;
}
