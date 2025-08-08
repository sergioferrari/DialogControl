#include <winlamb/window_main.h>
#include <winlamb/dialog_control.h>
#include <winlamb/statusbar.h>
#include <winlamb/menu.h>
#include <winlamb/button.h>
#include "../res/resource.h"
#include <memory>

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
		setup.title = L"WinLamb Sample";
		setup.position = { 400, 150 };
		setup.size = { 600, 400 };
		setup.style |= WS_MINIMIZEBOX;
		setup.style |= CS_HREDRAW | CS_VREDRAW;
		setup.wndClassEx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		setup.wndClassEx.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

		// assign the menu to the window
		setup.wndClassEx.lpszMenuName = MAKEINTRESOURCEW(IDR_MAINMENU);
		m_menu = GetMenu(this->hwnd());

		initStatusBar();
		initWinMessages();
		initAppCommands();
	}

	void initStatusBar();
	void initWinMessages();
	void initAppCommands();
	
private: 	
	wl::statusbar m_statusbar;
	wl::menu m_menu;
	std::unique_ptr<wl::dialog_control> m_client_dlg = nullptr;

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

}

void MainWindow::initAppCommands()
{
	on_command(ID_FILE_VIEWDIALOGA, [this](wl::params p) -> LRESULT {
		if (m_client_dlg && ::IsWindow(m_client_dlg->hwnd())) {
			::DestroyWindow(m_client_dlg->hwnd());
			m_client_dlg.reset();
		}
		else
		{
			m_client_dlg = std::make_unique<DialogA>();
			m_client_dlg->create(this->hwnd(), IDD_DIALOG_A, { 0,0 }, { 600,400 });
		}
	
		return 0;
	});

	on_command(ID_FILE_VIEWDIALOGB, [this](wl::params p) -> LRESULT {
			if (m_client_dlg && ::IsWindow(m_client_dlg->hwnd())) {
				::DestroyWindow(m_client_dlg->hwnd());
				m_client_dlg.reset();
				m_client_dlg = nullptr;
			}
			else
			{
				m_client_dlg = std::make_unique<DialogA>();
				m_client_dlg->create(this->hwnd(), IDD_DIALOG_A, { 0,0 }, { 600,400 });
			}
		return 0;
		});

	on_command(ID_FILE_EXIT, [this](wl::params p) -> LRESULT {
		PostQuitMessage(0); // Post a close message to the main window
		return 0;
		});
}

void MainWindow::initStatusBar()
{
	m_statusbar.create(this->hwnd());
	m_statusbar.add_fixed_part(100); 
	m_statusbar.add_resizable_part(1); 
	m_statusbar.add_fixed_part(100); 
	m_statusbar.set_text(L"Ready", 0); 
}


RUN(MainWindow);