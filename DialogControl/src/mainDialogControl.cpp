#include <winlamb/window_main.h>
#include <winlamb/dialog_control.h>
#include <winlamb/statusbar.h>
#include <winlamb/menu.h>
#include <winlamb/button.h>
#include "../res/resource.h"

class DialogA : public wl::dialog_control
{
public: 
	DialogA()
	{
		this->setup.dialogId = IDD_DIALOG_A;
		
		on_message(WM_INITDIALOG, [this](wl::params p) -> INT_PTR 
			{
				buttonOk.assign(this->hwnd(), IDOK);
				return TRUE;
			});

		this->on_command(IDOK, [this](wl::params p) -> INT_PTR {
			::EndDialog(this->hwnd(), IDOK);  // Chiude la finestra di dialogo
			return TRUE;
			});
	}

	virtual ~DialogA() = default;

private: 
	wl::button buttonOk;
};

class DialogB : public wl::dialog_control
{
public:
	DialogB()
	{
		this->setup.dialogId = IDD_DIALOG_B;

		on_message(WM_INITDIALOG, [this](wl::params p) -> INT_PTR
			{
				buttonOk.assign(this->hwnd(), IDOK);
				return TRUE;
			});

		this->on_command(IDOK, [this](wl::params p) -> INT_PTR {
			::EndDialog(this->hwnd(), IDOK);  // Chiude la finestra di dialogo
			return TRUE;
			});
	}

	virtual ~DialogB() = default;

private:
	wl::button buttonOk;
};


// MainWindow class -------------------------------------

class MainWindow : public wl::window_main
{
public: 
	MainWindow()
	{
		setup.wndClassEx.lpszClassName = L"SampleMainWindow";
		setup.title = L"DialogControl Sample";
		setup.position = { 400, 150 };
		setup.size = { 600, 400 };
		setup.style |= WS_MINIMIZEBOX;
		setup.style |= CS_HREDRAW | CS_VREDRAW;
		setup.wndClassEx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		setup.wndClassEx.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

		// assign the menu to the window
		setup.wndClassEx.lpszMenuName = MAKEINTRESOURCEW(IDR_MAINMENU);
		m_menu = GetMenu(this->hwnd());

		initWinMessages();
		initAppCommands();
	}

	void initWinMessages();
	void initAppCommands();

	void initStatusBar()
	{
		m_statusbar.create(this->hwnd());
		m_statusbar.add_fixed_part(100); // fixed width part
		m_statusbar.add_resizable_part(1); // resizable part with weight 1
		m_statusbar.add_fixed_part(100); // another fixed width part
		m_statusbar.style.size_grip(true); // enable size grip
		m_statusbar.set_text(L"Ready", 0); // set text for the first part
	}


private: 	
	wl::statusbar m_statusbar;
	wl::menu m_menu;
	DialogA dialogA;
	DialogB dialogB;

};

void MainWindow::initWinMessages()
{
	on_message(WM_CREATE, [this](wl::params p) -> LRESULT {
		initStatusBar(); // Initialize the status bar when the window is created
		return 0;
	});
	on_message(WM_DESTROY, [](wl::params p) -> LRESULT {
		PostQuitMessage(0); // Post quit message when the window is destroyed
		return 0;
		});
	on_message(WM_SIZE, [this](wl::params p) -> LRESULT {
		m_statusbar.adjust(p); // Adjust the status bar on window resize
		return 0;
	});
	on_message(WM_COMMAND, [this](wl::params p) -> LRESULT {
		if (LOWORD(p.wParam) == ID_FILE_EXIT) {
			PostQuitMessage(0); // Exit the application
			return 0;
		}
		return DefWindowProcW(this->hwnd(), p.message, p.wParam, p.lParam);
		});
}

void MainWindow::initAppCommands()
{
	on_command(ID_FILE_VIEWDIALOGA, [this](wl::params p) -> LRESULT {
		dialogA.create(this->hwnd(), IDD_DIALOG_A, {0,0}, {600,400});
		return 0;
	});

	on_command(ID_FILE_VIEWDIALOGB, [this](wl::params p) -> LRESULT {
		dialogB.create(this->hwnd(), IDD_DIALOG_B, { 0,0 }, { 600,400 });
		return 0;
		});
}


RUN(MainWindow);