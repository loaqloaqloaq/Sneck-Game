//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Apple.h"
#include "Task_Player.h"
#include "sound.h"

namespace  Apple
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->appleImg = DG::Image::Create("./data/image/ringo32x32.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		this->appleImg.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();
		
		//f[^ϊ»
		this->render2D_Priority[1] = 0.5f;
		this->hitBase = ML::Box2D(-32 / 2, -32 / 2, 32, 32);
		se::LoadFile("poku", "./data/sound/poku.wav");
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ
		ge->appleCnt[this->type]--;

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		this->moveCnt++;
		if (this->moveCnt > 300 && this->type == 1 && ge->status == ge->Status::End) {
			this->Kill();
		}
		this->CheckHit();
	}
	//-------------------------------------------------------------------
	//ΥΛ»θ
	void Object::CheckHit() {
		auto pl = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		auto playerHitBox = pl->hitBase.OffsetCopy(pl->pos);
		auto appleHitBox= this->hitBase.OffsetCopy(this->pos);
		if (playerHitBox.Hit(appleHitBox)) {
			if (this->type == 0) ge->score++;
			else ge->score--;
			pl->speed++;			
			se::Play("poku");
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{

		ML::Box2D appleSrc[2] = {
			ML::Box2D(0, 0, 32, 32),
			ML::Box2D(32, 0, 32, 32),
		},appleDraw;
		appleDraw = this->hitBase.OffsetCopy(this->pos);
		
		this->res->appleImg->Draw(appleDraw, appleSrc[this->type]);

	}
	
	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^

			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//\[XNXΜΆ¬
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}