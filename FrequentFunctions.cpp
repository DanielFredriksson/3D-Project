#include "FrequentFunctions.hpp"

LRESULT CALLBACK WindowsProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
)
{

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CHAR:								// TranslateMessage has recieved a character message,
		switch (wParam) {						// which is: Any Character, Backspace, Enter, ESC,
		default:			//Any Character.	// Shift+Enter, Tab
			TCHAR ch;		//Character string that can described ANSI, DBCS, UNICODE strings.
			ch = wParam;
			if (ch == 'w') {

			}
			if (ch == 'a') {

			}
			if (ch == 's') {

			}
			if (ch == 'd') {

			}

			break;
		}
		break;

	case WM_MOUSEMOVE: {

		break;
	}

	}

	return DefWindowProc(hWnd, message, wParam, lParam);	// If we process SYSKEYSTROKES, which
															// are ALT+KeyStroke, this function
															// allows windows to keep processing normal
															// SYSKEYSTROKES, such as ALT-TAB.
}
