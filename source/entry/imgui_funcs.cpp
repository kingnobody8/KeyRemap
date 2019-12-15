#include "entry/imgui_funcs.h"
#include "entry/sfml_render.h"
#include "third_party/imgui/imgui.h"
#include "third_party/imgui/imgui_internal.h"
#include "sfml/Graphics/GLCheck.hpp"
#include "sfml/OpenGL.hpp"
#include "sfml/Graphics.hpp"

ImGuiContext* pCtx = nullptr;
sf::Texture* pFontTexture;

void InitImgui()
{
	// Application init
	pCtx = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	auto* pRenWin = GetRenWin();
	io.DisplaySize.x = pRenWin->getSize().x;
	io.DisplaySize.y = pRenWin->getSize().y;
	//io.IniFilename = "imgui.ini";

	// Load texture atlas - there is a default font so you don't need to care about choosing a font yet
	unsigned char* pixels = nullptr;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	pFontTexture = new sf::Texture();
	pFontTexture->create(width, height);
	pFontTexture->update(pixels);
	io.Fonts->TexID = (void*)pFontTexture->getNativeHandle();
	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();


	//setup style
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	style.Colors[ImGuiCol_PopupBg].w = 1.0f;

	/*io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
	io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
	io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
	io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
	io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
	io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
	io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
	io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
	io.KeyMap[ImGuiKey_Insert] = sf::Keyboard::Insert;
	io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
	io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
	io.KeyMap[ImGuiKey_Space] = sf::Keyboard::Space;
	io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Enter;
	io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
	io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
	io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
	io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
	io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
	io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
	io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;*/
}

void InputImgui()
{
	ImGui::NewFrame();
}

void DoIOImgui()
{
	static bool b = true;
	ImGui::ShowDemoWindow(&b);
}

void DrawImgui(sf::RenderTarget* apTarget)
{
	apTarget->pushGLStates();

	//  render & swap video buffers
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Render();
	auto pDrawData = ImGui::GetDrawData();

	// scale stuff (needed for proper handling of window resize)
	int fb_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width == 0 || fb_height == 0) { return; }
	pDrawData->ScaleClipRects(io.DisplayFramebufferScale);

	// We are using the OpenGL fixed pipeline to make the example code simpler to read!
	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
	GLint last_texture; glCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture));
	GLint last_viewport[4]; glCheck(glGetIntegerv(GL_VIEWPORT, last_viewport));
	GLint last_scissor_box[4]; glCheck(glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box));
#ifndef SFML_OPENGL_ES
	glCheck(glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT));
#else
	GLboolean bBlendEnabled = glIsEnabled(GL_BLEND);
	GLboolean bCullEnabled = glIsEnabled(GL_CULL_FACE);
	GLboolean bDepthEnabled = glIsEnabled(GL_DEPTH_TEST);
	GLboolean bScissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
	GLboolean bTextureEnabled = glIsEnabled(GL_TEXTURE_2D);
#ifdef DEBUG
	glCheck(bool b = false); //catch any errors
#endif
#endif
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glCheck(glDisable(GL_CULL_FACE));
	glCheck(glDisable(GL_DEPTH_TEST));
	glCheck(glEnable(GL_SCISSOR_TEST));
	glCheck(glEnable(GL_TEXTURE_2D));
	//glCheck(glEnableClientState(GL_VERTEX_ARRAY));
	//glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
	//glCheck(glEnableClientState(GL_COLOR_ARRAY));
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context

	// Setup viewport, orthographic projection matrix
	glCheck(glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height));
	glCheck(glMatrixMode(GL_PROJECTION));
	glCheck(glLoadIdentity());
#ifndef SFML_OPENGL_ES
	glCheck(glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f));
#else
	glCheck(glOrthof(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f));
#endif
	glCheck(glMatrixMode(GL_MODELVIEW));
	glCheck(glLoadIdentity());

	glCheck(glMatrixMode(GL_TEXTURE));
	glCheck(glLoadIdentity());

	//draw everything
	for (int n = 0; n < pDrawData->CmdListsCount; ++n)
	{
		const ImDrawList* cmd_list = pDrawData->CmdLists[n];
		const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
		const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();

		glCheck(glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + offsetof(ImDrawVert, pos))));
		glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + offsetof(ImDrawVert, uv))));
		glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + offsetof(ImDrawVert, col))));

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); ++cmd_i)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				GLuint tex_id = (GLuint)*((unsigned int*)&pcmd->TextureId);
				glCheck(glBindTexture(GL_TEXTURE_2D, tex_id));
				glCheck(glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w),
					(int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y)));
				glCheck(glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer));
			}
			idx_buffer += pcmd->ElemCount;
		}
	}

	// Restore modified state
	//glCheck(glDisableClientState(GL_COLOR_ARRAY));
	//glCheck(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
	//glCheck(glDisableClientState(GL_VERTEX_ARRAY));
	//glCheck(glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture));
#ifndef SFML_OPENGL_ES
	glCheck(glPopAttrib());
#else
	//glPopAttrib doesn't exist for GLES -> so we manage the state ourselves

#define ENA_DIS(enum_val, bool_val) \
				if(bool_val) { glCheck(glEnable(enum_val)); } \
				else { glCheck(glDisable(enum_val)); }

	ENA_DIS(GL_BLEND, bBlendEnabled);
	ENA_DIS(GL_CULL_FACE, bCullEnabled);
	ENA_DIS(GL_DEPTH_TEST, bDepthEnabled);
	ENA_DIS(GL_SCISSOR_TEST, bScissorEnabled);
	ENA_DIS(GL_TEXTURE_2D, bTextureEnabled);
#endif
	glCheck(glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]));
	glCheck(glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]));

	apTarget->popGLStates();
	apTarget->resetGLStates();
}

void ExitImgui()
{

}