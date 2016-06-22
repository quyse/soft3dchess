This was my coursework in Computer Graphics discipline in 2009.

[Game on itch.io](https://quyse.itch.io/soft3dchess)

* No AI, multiplayer only;
* Features homemade multi-core software 3D renderer, so it doesn't depend on OpenGL, DirectX or graphics drivers; it will hot up your CPU instead of GPU!
* Supports network game (local network only).

To start a game, select one of the commands in the Play menu. "Play with yourself" means playing on the single PC simply doing moves one player after another.

Hold right mouse button to rotate camera.

Game runs only under Windows, because it uses WinAPI for everything, and I'm too lazy to rewrite it now.

Code features:

* Graphics pipeline made via C++ templates and generic vertex/pixel shader system;
* Automatic work balancing between available CPU cores.

On the other hand, code is not great in every aspect, for example it doesn't do perspective correction of vertex shader outputs, and a few other things.

Assets (models of chess figures) has been found freely on the internet back then, and now I have no idea where; if you know, let me know too so I can give proper credits.
