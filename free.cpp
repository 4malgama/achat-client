class AuthorizationWidget;

namespace auth { extern AuthorizationWidget* w; }
namespace aes { void free(); }

namespace unload
{
	void free()
	{
		aes::free();
	}
}
