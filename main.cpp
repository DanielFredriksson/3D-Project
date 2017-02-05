


//--------------------------------------------------------------------------------------
//	DirectX 11 Starting Template Created by Stefan Petersson (BTH) 2014
//	Project Coded by Daniel Fredriksson & Oliver Glandberger.
//--------------------------------------------------------------------------------------
// APPLICATION
#include "ApplicationClass.hpp"

// FOR MEMORY LEAKS
#include <crtdbg.h>


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ObjectHandlerClass has not implemented deletion of ObjectArray
	//





	ApplicationClass Application;
	Application.Initialize(hInstance);

	



	//Application.SetObjectData();


	ShowWindow(Application.GetWNDHandle(), nCmdShow);

	while (Application.GetInputMessage()->message != WM_QUIT)
	{

		if (PeekMessage(Application.GetInputMessage(), nullptr, 0, 0, PM_REMOVE))
		{


			TranslateMessage(Application.GetInputMessage());	// Calls WindowProcedure in some part of its internal process.
			DispatchMessage(Application.GetInputMessage());


			Application.UpdateCamera();


		}
		else
		{

			
			// Application.RenderDeferred();
			Application.RenderAll();

			// Application.RenderDeferred();
			//

			Application.Present();

		}
	}

	Application.Shutdown();

	return (int)(*Application.GetInputMessage()).wParam;
}
