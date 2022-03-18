BOOL WINAPI DllMain( HMODULE hDll, DWORD dwReason, PVOID lpReserved)
{
//	if ( dwReason == DLL_PROCESS_ATTACH )
//	{
		//DeleteFileA( LOG_FILE_PATH );
	//VMProtectBegin("point_1");
	//thread_running = 0;
	/*HANDLE m_hMutex = CreateMutex(NULL, FALSE, "fkblueshitfk");//
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		OutputDebugString("fkblueshitfk exist\n");
		CloseHandle(m_hMutex);
		return 0;
	}*/
	VMProtectBegin("start1");
	aimbot_thread_running = 0;
	if (thread_running==1)
	{
		OutputDebugString("only run one\n");
		return 1;

	}
	ULONG64 func = (ULONG64)GetProcAddress(GetModuleHandle("User32.dll"), "GetAsyncKeyState");
	if (func == 0)
	{
		MessageBox(0, "???@##@", 0, 0);
		return 0;
	}
	func += 5; //GetAsyncKeyState
	GetAsyncKeyState_call = (GetAsyncKeyState_)func;
	 is_active = 0;
	is_press_key = 0;
	aim_target_addr = 0;
	s_width = GetSystemMetrics(SM_CXSCREEN);
	s_height = GetSystemMetrics(SM_CYSCREEN);
	VMProtectEnd();
		_beginthreadex( nullptr, NULL, MainThread, nullptr, NULL, nullptr );

	//}
		//VMProtectEnd();
	return true;
}
UINT WINAPI MainThread( PVOID )
{
	//OutputDebugString("start fun\n");;
	/*SetSePrivilege();
	if (open_map_data()!=0)
	{
		OutputDebugString("open map ptr failed\n");
		return 0;

	}*/
	//VMProtectBegin("start1");
	thread_running = 1;
	if (create_map_1()!=0)
	{
		OutputDebugString("create map failled\n");
		return 0;
	}
	game_ptr->is_active = 0;
	game_ptr->loop_count = 0;
	game_ptr->exit_code = 0;
	/*if (create_map_2() != 0)
	{
		OutputDebugString(" create2 map failled\n");
		return 0;
	}*/
	/*while (true)
	{   
		if (ptr_x64_pass_code->code==0xCC)
		{
			break;
		}
		Sleep(10);
	}*/
	MH_Initialize();

	while ( !GetModuleHandleA( "dwmcore.dll" ) )
		Sleep( 150 );

	//
	// [ E8 ? ? ? ? ] the relative addr will be converted to absolute addr
	auto ResolveCall = []( DWORD_PTR sig )
	{
		return sig = sig + *reinterpret_cast< PULONG >( sig + 1 ) + 5;
	};

	//
	// [ 48 8D 05 ? ? ? ? ] the relative addr will be converted to absolute addr
	auto ResolveRelative = []( DWORD_PTR sig )
	{
		return sig = sig + *reinterpret_cast< PULONG >( sig + 0x3 ) + 0x7;
	};

	auto dwRender = FindPattern( "d2d1.dll", PBYTE( "\x48\x8D\x05\x00\x00\x00\x00\x33\xED\x48\x8D\x71\x08" ), "xxx????xxxxxx" );
	if (dwRender==0)
	{
		MessageBox(0, "operating system version not supported", 0, 0);
		return 0;

	}
	ULONG64 dwRender_addr = dwRender;
	if ( dwRender )
	{
		dwRender = ResolveRelative( dwRender );

		PDWORD_PTR Vtbl = PDWORD_PTR( dwRender );
		//ZeroMemory(out_debug, 64);
	//	sprintf_s(out_debug, "dwRender:%llx,Vtbl:%llx\n", dwRender, dwRender_addr);
	//	OutputDebugString(out_debug);
		//OutputDebugString( "table 0x%llx\n", dwRender);
		//OutputDebugString("")
		//VMProtectEnd();
		MH_CreateHook( PVOID( Vtbl[ 6 ] ), PVOID( &hkPresentDWM ), reinterpret_cast< PVOID* >( &oPresentDWM ) );
		MH_CreateHook( PVOID( Vtbl[ 7 ] ), PVOID( &hkPresentMPO ), reinterpret_cast< PVOID* >( &oPresentMPO ) );

		ID3D11Device* d3d11dev;
		ID3D11DeviceContext* d3d11devcon;
		PVOID* pInterfaceVtable = NULL;


		//Gdi32.dll
		ULONG64 gdi32_base = (ULONG64)GetModuleHandle("Gdi32.dll");
		ULONG64 BitBlt_addr = (ULONG64)GetProcAddress((HMODULE)gdi32_base, "BitBlt");
		if (BitBlt_addr ==0)
		{
			MessageBox(0, "get bitbit failed", 0, 0);
			return 0;
		}
		MH_CreateHook(reinterpret_cast<LPVOID>(BitBlt_addr), &hkBitBlt, reinterpret_cast<PVOID*>(&BitBlt_call));



		HRESULT hresult = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &d3d11dev, nullptr, &d3d11devcon);
		if (hresult!=0)
		{
			OutputDebugString("hkfailid\n");
			return 0;

		}
		pInterfaceVtable = *reinterpret_cast<PVOID**>(d3d11dev);
		MH_CreateHook(PVOID(pInterfaceVtable[5]), PVOID(&hkCreateTexture2D), reinterpret_cast<PVOID*>(&CreateTexture2D_call));

		MH_EnableHook( MH_ALL_HOOKS );

	//	VMProtectBegin("start2");
		//VMProtectEnd();
		/*oPresentDWM =  (PresentDWM_)Vtbl[6];//(PresentDWM_)oPresentDWM_origin;
		oPresentMPO = (PresentMPO_)Vtbl[7];
		hook_address1= Vtbl[6];
		hook_address2= Vtbl[7];
		jmp_address1 = (ULONG64)&hkPresentDWM;
		jmp_address2 = (ULONG64)&hkPresentMPO;
		ZeroMemory(out_debug, 64);
		sprintf_s(out_debug, "hook 1=%llx tid:%d hook2 %llx\n", hook_address1, GetCurrentThreadId(),(ULONG64)hook_address2);
		OutputDebugString(out_debug);*/
		//AddVectoredExceptionHandler(1, PvectoredExceptionHandler);
		//VirtualProtect((LPVOID)hook_address1, 1, PAGE_EXECUTE_READWRITE, &old_protect);
		//*(BYTE*)hook_address1 = 0xcc;
		//VirtualProtect((LPVOID)hook_address1, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &old_protect);
		//HWBreakPointHook(hook_address2);
		//RefreshHooks(hook_address1);
	//	RefreshHooks(hook_address2);
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hardware_breakpoint_Thread, 0, 0, 0);
		//OutputDebugString("HOOK finish\n");

	}
	else
	{
		OutputDebugString("not found dwRender\n");
	}
	return 0;
}
void ShowEsp()
{
	//D3DCOLOR_XRGB(255, 0, 0);
	//out player
	if (isscreenshot==1)
	{
		if (game_ptr==0)
		{
			isscreenshot = 0;
			return;
		}
		if (game_ptr->is_active==0)
		{
			isscreenshot = 0;
			return;
		}
		//render.RenderText("s.h.o.t.1", 0, 0, -1, 0, 0);
		//return;
	}
	/*if (isscreenshot == 2)
	{
		render.RenderText("s.h.o.t.2", 0, 0, -1, 0, 0);
		return;
	}*/
	float DonguId = -1;
	float DonguDist = -1;
	if (game_ptr==0)
	{
		return;
	}
	if (game_ptr->is_active==0)
	{
		//render.OutlineRect(0, 0, 100, 100, D3DCOLOR_ARGB(64, 128, 0, 0));
		//render.FillRect(0, 0, 100, 100, D3DCOLOR_ARGB(255, 0, 255, 000));
		//render.DrawLine(XMFLOAT2(0, 0), XMFLOAT2(500, 500), D3DCOLOR_ARGB(254, 254, 0, 0));//yellow
		//render.DrawLine(XMFLOAT2(0, 0), XMFLOAT2(200, 200), D3DCOLOR_ARGB(254, 10, 254, 254));//blue
		render.RenderText(L"wait--for--activated", 0, 0, -1, 0, 0);
		render.RenderText(L"--press end exit--", 0, 25, -1, 0, 0);
		if (game_ptr->loop_count==0)
		{
			return;
		}
	}
	else
	{
		//render.RenderText("PUBG SK activated", 0, 0, -1, 0, 0);
	}
	render.RenderText(L"--activated--", 0, 15, -1, 0, 0);
	ZeroMemory(out_SpectatedCount, 32);
	ZeroMemory(out_viewr_count_txt, 64);
	sprintf_s(out_SpectatedCount, "--spectator:%d", game_ptr->SpectatedCount);
	mbstowcs(out_viewr_count_txt, out_SpectatedCount, strlen(out_SpectatedCount));
	render.RenderText(out_viewr_count_txt, 0, 30, -1, 0, 0);
	if (select_aim_type==0)
	{
		render.RenderText(L"--F5-aim:head", 0, 45, -1 , 0, 0);
	}
	else
	{
		render.RenderText(L"--F5-aim:chest", 0, 45, -1 , 0, 0);
	}
	render.RenderText(L"--F1-show-name", 0, 60, -1, 0, 0);
	ZeroMemory(out_smooth_txt, sizeof(out_smooth_txt));
	wsprintfW(out_smooth_txt, L"--F2-smooth--speed:%d\n", aim_smooth);
	render.RenderText(out_smooth_txt, 0, 75, -1, 0, 0);

	ZeroMemory(out_aim_range_txt, sizeof(out_aim_range_txt));
	wsprintfW(out_aim_range_txt, L"--F3-aim-range:%d\n",(int)aim_range);
	render.RenderText(out_aim_range_txt, 0, 90, -1, 0, 0);
	if (aim_key == VK_CAPITAL)
	{
		render.RenderText(L"--F4-aim:caps", 0, 105, -1, 0, 0);
	}
	else
	{
		render.RenderText(L"--F4-aim:mouse right", 0, 105,-1, 0, 0);
	}
	ZeroMemory(out_aim_dis_txt, sizeof(out_aim_dis_txt));
	wsprintfW(out_aim_dis_txt, L"--F6-a.i.m-dist:%d\n", (int)aim_dist);
	render.RenderText(out_aim_dis_txt, 0, 120, -1, 0, 0);
	render.RenderText(L"--press end exit--", 0, 135, -1, 0, 0);
//	ZeroMemory(out_text, 64);
//	sprintf_s(out_text, " p.l.a.y.e.r :%d", game_ptr->loop_count);
//	render.RenderText(out_text, 0, 12, -1, 0, 0);
	/*ZeroMemory(out_text, 64);
	ZeroMemory(out_SpectatedCount, 32);
	sprintf_s(out_SpectatedCount, "SpectatedCount:%d", game_ptr->SpectatedCount);
	sprintf_s(out_text, "player:%d", game_ptr->loop_count);
	render.RenderText(out_text, 0, 12, -1, 0, 0);
	render.RenderText(out_SpectatedCount, s_width/2, 0, -1, 0, 0);*/
	/*if (ptr_x64_pass_code->code != 0xCC)
	{
		return;
	}*/
	ULONG color_player;
	ULONG box_color;
	for (int i = 0; i < game_ptr->loop_count; i++)
	{
		  // ZeroMemory(out_text, 64);
		//	sprintf_s(out_text, " speed:%d", game_ptr->InitialSpeed);
		//	render.RenderText(out_text, 0, 200, -1, 0, 0);
		//FOV = tanf(DEG2RAD(5.f) / 2) / tanf(DEG2RAD(game_ptr->FOV) / 2) * (s_width);
		//Circle((int)s_width / 2, (int)s_height / 2, FOV, 30);
		//ZeroMemory(out_SpectatedCount, 32);
		//sprintf_s(out_SpectatedCount, "SpectatedCount:%d", game_ptr->SpectatedCount);
		//render.RenderText(out_SpectatedCount, s_width / 2, 0, -1, 0, 0);

		/*ZeroMemory(out_SpectatedCount, 32);
		sprintf_s(out_SpectatedCount, "S.p.e.c.t.a.t.e.d.C.o.u.n.t:%d", game_ptr->SpectatedCount);
		render.RenderText(out_SpectatedCount, s_width / 2, 0, -1, 0, 0);*/

	//	Vector3 player_Velocity  = game_ptr->Velocity[i];
		//Vector3 player_Velocity = { 0,0,0 };
		Vector3 chest_pos= GetBoneWithRotation_new(game_ptr->bone_chest[i], game_ptr->ComponentToWorld[i]);
		Vector3 waist_pos = GetBoneWithRotation_new(game_ptr->bone_waist[i], game_ptr->ComponentToWorld[i]);
		Vector3 head_pos = GetBoneWithRotation_new(game_ptr->bone_head[i], game_ptr->ComponentToWorld[i]);
		Vector3 bone_left_foot_pos = GetBoneWithRotation_new(game_ptr->bone_left_foot[i], game_ptr->ComponentToWorld[i]);
		Vector3 bone_right_foot = GetBoneWithRotation_new(game_ptr->bone_right_foot[i], game_ptr->ComponentToWorld[i]);

		Vector3 bone_right_foot_pos_2d= WorldToScreen(bone_right_foot, game_ptr->localtion, game_ptr->Rotation, game_ptr->FOV, s_width, s_height);
		Vector3 bone_left_foot_pos_2d = WorldToScreen(bone_left_foot_pos, game_ptr->localtion, game_ptr->Rotation, game_ptr->FOV, s_width, s_height);
		Vector3 chest_2d = WorldToScreen(chest_pos, game_ptr->localtion, game_ptr->Rotation, game_ptr->FOV, s_width, s_height);
		Vector3 waist_pos_2d = WorldToScreen(waist_pos, game_ptr->localtion, game_ptr->Rotation, game_ptr->FOV,s_width,s_height);
		Vector3 head_pos_2d= WorldToScreen(head_pos, game_ptr->localtion, game_ptr->Rotation, game_ptr->FOV, s_width, s_height);
		float distance = (bone_left_foot_pos - game_ptr->localtion).Length() / 100.f;
		if (distance <= 600.f)
		{
			if (game_ptr->actor_hp[i] == 1)//invisible
			{
				color_player = D3DCOLOR_ARGB(255, 0, 255, 000);
				box_color = D3DCOLOR_ARGB(254, 10, 254, 254);//blue
				
			}
			else
			{
				color_player = D3DCOLOR_ARGB(255, 20, 255, 255);//D3DCOLOR_XRGB(255, 255, 255);//golden
				box_color = D3DCOLOR_ARGB(254, 254, 0, 0);//yellow
				////visible
			}
			ZeroMemory(distance_txt, 64);
			ZeroMemory(dist_txt, 64);
			ZeroMemory(out_name_dist, 64);
			if (DisId==i)
			{
				sprintf_s(distance_txt, " %0.fm--Lock", distance);
			}
			else
			{
				sprintf_s(distance_txt, " %0.fm", distance);
			}
			mbstowcs(dist_txt, distance_txt, strlen(distance_txt));
		//	strcat_s(out_name_dist, game_ptr->player_name[i].name);
		//	strcat_s(out_name_dist, distance_txt); 
			if (show_name==1)//show name
			{
				wcscat(out_name_dist, game_ptr->player_name[i].name);
			}
			wcscat(out_name_dist, dist_txt);
			//render.RenderText(out_name_dist, bone_left_foot_pos_2d.x, bone_left_foot_pos_2d.y, color_player, 0, 0);//distance_txt

			//////////////////////draw name & distance

			char team_id[32];
			ZeroMemory(team_id, 32);
			sprintf_s(team_id, "team:%d", game_ptr->team_id[i]);
			ZeroMemory(team_id_text, 64);
			mbstowcs(team_id_text, team_id, strlen(team_id));

			char view_count_txt[32];
			ZeroMemory(view_count_txt, 32);
			sprintf_s(view_count_txt, "view:%d", game_ptr->player_vide[i]);
			ZeroMemory(view_txt, 64);
			mbstowcs(view_txt, view_count_txt, strlen(view_count_txt));

			ZeroMemory(out_all_info, sizeof(out_all_info));
			wcscat(out_all_info, out_name_dist);
			wcscat(out_all_info, L"\n");
			wcscat(out_all_info, team_id_text);
			wcscat(out_all_info, L" ");
			wcscat(out_all_info, view_txt);
			render.RenderText(out_all_info, bone_left_foot_pos_2d.x, bone_left_foot_pos_2d.y, color_player, 0, 0);//distance_txt

			//ZeroMemory(game_ptr->player_name[i].name, 32);

			float height = bone_left_foot_pos_2d.y-head_pos_2d.y;
			float width = height / 2.0f;
			render.OutlineRect(head_pos_2d.x - (width / 2), head_pos_2d.y, width, height,box_color);
		}
		if (DisId==i)
		{
			if (game_ptr->actor_hp[i] == 1)//invisible
			{
				DisId = -1;

			}

		}

		Vector3 recoil_fix;
		Vector3 Rotation;
		Vector3 Prediction_fix;
		float boneX; float boneY; float bone_x; float bone_y;
		if (distance <= 600.f && game_ptr->actor_hp[i] == 0)//0===visible 1==invisible
		{
			if (GetAsyncKeyState_call(aim_key) & 0x8000)//
			{
			   


				recoil_fix = game_ptr->recoil;
				if (recoil_fix.Length() > 0.25f)
				{
					recoil_fix.y -= game_ptr->LeanLeft / 4.0f;
					recoil_fix.y += game_ptr->LeanRight / 4.0f;

				}
				Vector3 player_Velocity = game_ptr->Velocity[i];
				Vector3 aim_pos;
				if (select_aim_type==0)
				{
					aim_pos = head_pos;
				}
				else
				{
					aim_pos = chest_pos;
				}
				Vector3 aim= Prediction(aim_pos, player_Velocity, distance);//head_pos
				Rotation = game_ptr->sway + recoil_fix;//may need fix

				head_aim = WorldToScreen(aim, game_ptr->localtion, Rotation, game_ptr->FOV, s_width, s_height);///Rotation
				boneX = head_aim.x - s_width / 2;
				boneY = head_aim.y - s_height / 2;
				if (boneX >= -aim_range && boneX <= aim_range && boneY >= -aim_range && boneY <= aim_range && distance <= aim_dist&&game_ptr->actor_hp[i] == 0)//
				{
					
					
				
					if (DisId == -1)
					{
						DisId = i;
					}

				
					if (DisId == i)
					{
						
						if (aimbot_thread_running == 0)
						{
							global_bone_x = boneX;
							global_bone_y = boneY;
							CreateThread(0, 0, (LPTHREAD_START_ROUTINE)aimbot_thread, 0, 0, 0);
						}
					}


				}
			
			}

		}
	
	}
	DisId = -1;
	ZeroMemory(game_ptr->player_name, sizeof(game_ptr->player_name));
}
