#include "Form.h"
#include "D3DInterface.h"
#include "BaseShader.h"
#include "InstanceChannelList.h"
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, PSTR psCmdLine, int iCmdshow)
{
	InstanceChannelList list =  InstanceChannelList();
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

	bool needsRendered[] = { false };

	int sizes[] = { 100 };

	LPCWSTR textureNames[] = { L"Smile.dds" };

	float distance = 0.0f;


	//init the instance channel list
	list.Initialize(needsRendered, sizes, 1, textureNames, d3d);

	Instance* instance = new Instance(BaseShader::BaseShaderInstance(0, 0, 100, 100), 0);

	//add the instance to the list
	list.AddInstance(0, instance);

	shader->updateConstantBuffer(d3d, -200.0f*form->GetPixelWidth(), -200.0f*form->GetPixelHeight());

	//
	while (!form->GetRecievedQuitMessage())
	{
		form->Update();
		//begin drawing
		d3d->BeginDrawing(1, 0, 0);

		//draw the shader
		//shader->Render(d3d);
		if(instance)
		instance->SetInstanceData( BaseShader::BaseShaderInstance(distance += 0.01, 0, 100, 100));

		//render the list
		shader->RenderInstance(d3d, list.GetInstanceBuffer(d3d, 0), list.GetTexture(0), list.getListCount(0));

		if (distance > 0.3 && instance)
		{
			//remove instace from the list
			list.DeleteInstance(instance, 0);
			delete instance;
			instance = NULL;
			
		}

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
