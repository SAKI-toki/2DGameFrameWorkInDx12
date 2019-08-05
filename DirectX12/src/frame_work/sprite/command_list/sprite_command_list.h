/**
* @file sprite_command_list.h
* @brief �X�v���C�g�p�̃R�}���h���X�g�N���X
* @author �ΎR�@�I
* @date 2019/03/12
*/
#pragma once
#include "../../command_list/Execute/execute_command_list.h"
#include <saki/singleton.h>

/**
* @brief �X�v���C�g�p�̃R�}���h���X�g�N���X
*/
class SpriteCommandList :public ExecuteCommandList, public saki::singleton<SpriteCommandList>
{
protected:
	HRESULT CreatePipeline()override;
};