#include "Form.h"
#include "D3DInterface.h"
#include "BaseShader.h"
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, PSTR psCmdLine, int iCmdshow)
{
	Form* form = new Form();
	D3DInterface* d3d = new D3DInterface();
	BaseShader* shader = new BaseShader();

	//create the window
	form->InitWindowsForm(1280, 960, false, L"2D platformer");

	if (!form->IsInitialized())
	{
		return 0;
	}

	//initialize the interface
	d3d->InitializeD3D(true, form);

	shader->InitShader(d3d, form);


	shader->updateConstantBuffer(d3d, -200.0f*form->GetPixelWidth(), -200.0f*form->GetPixelHeight());

	//
	while (!form->GetRecievedQuitMessage())
	{
		form->Update();
		//begin drawing
		d3d->BeginDrawing(1, 0, 0);

		//draw the shader
		shader->Render(d3d);

		//present
		d3d->Present();
 		bool escapedPressed = form->GetKeyState(32);
		if (escapedPressed)
			break;
	}

	//delete the d3d interface
	d3d->Close();
	delete d3d;

	form->Close();
	delete form;

	return 0;
}
