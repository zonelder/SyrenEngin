#include "App.h"
#include <cmath>
#include "../component/Transform.h"
#include "TransformUtils.h"
#include "Mouse.h"

const double PI = acos(-1.0);

App::App() :_wnd(800, 600, "engin win"),_gfx(_wnd.GetHWND())
{
	_mainCamera.aspectRatio = _wnd.GetWidth() / _wnd.GetHeight();
	_obj.push_back(Primitive::CreateBox());
	_obj.push_back(Primitive::createCylinder(24));
	_obj.at(1).transform.scale.y = 3;
}


int App::Init(){
	MSG msg;
	BOOL gResult;

	for (auto& obj : _obj)// Renderable object init
	{
		obj.InitBinds(_gfx);
	}

	while (true) {
		//if processMessage has a value then it means than we wanna exit from app
		if (const auto ecode = Window::processMessage()) {
			return *ecode;
		}
		SetMouseData(_wnd.mouseHandler);
		Update();
		Frame();// TODO better handle vector<servise> so inplementation of each servises can be separete from app class
	}

}

// App ����� ���� ������� ��������� � ���� � ������ ����� ����� � ��������� �� ��� �� � ��������� �������� � �������� ����� ���������� ������ ===))))))

void App::Update()
{

	float angle = _time.peek();
	_obj.at(0).transform.rotation = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, angle, angle);
	angle = -angle;
	float x = 2.0f * Mouse::GetNormedX() - 1.0f;
	float y = 300;
	float z = 2.0f * Mouse::GetNormedY() - 1.0f;
	_obj.at(1).transform.position.x = x;
	_obj.at(1).transform.position.z = z;
	_obj.at(1).transform.rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle, 0.0f, angle);
	float speed = 0.01f;
	if (Mouse::LeftKeyDown())// moving in left mouse button pressed
	{
		_mainCamera.transform.position.x -= speed * Mouse::GetDeltaX();
		_mainCamera.transform.position.y += speed * Mouse::GetDeltaY();
		_mainCamera.transform.position.z = -4.0f;
		return;
	}
	if (Mouse::RightKeyDown())// rotating in left mouse button pressed
	{
		float cam_yaw = -speed * Mouse::GetDeltaX();
		float cam_pitch = -speed * Mouse::GetDeltaY();
		_mainCamera.transform.rotation = DirectX::XMQuaternionMultiply(_mainCamera.transform.rotation, DirectX::XMQuaternionRotationRollPitchYaw(cam_pitch, cam_yaw, 0.0f));
		return;
	}

}

void App::Frame() {

	_mainCamera.OnFrame();
	_gfx.ClearBuffer(_mainCamera.background);

	for (auto& obj : _obj)
	{
		obj.orientationMatrix = DirectX::XMMatrixTranspose(
			toOrientationMatrix(obj.transform) *
			_mainCamera.orientationMatrix

		);
		obj.Draw(_gfx);
	}

	_gfx.endFrame();
}


void App::SetMouseData(const MouseHandler& mh)
{
	Mouse& m = Mouse::GetInstance();
	int x = mh.getPosX();
	int y = mh.getPosY();
	m._dx = x - m._x;
	m._dy = y - m._y;
	m._x = x;
	m._y = y;

	m._clampX = (float)x / _wnd.GetWidth();
	m._clampY = (float)y / _wnd.GetHeight();

	m._isLeftPressed = mh.LeftIsPressed();
	m._isRightPressed = mh.RightIsPressed();

}
