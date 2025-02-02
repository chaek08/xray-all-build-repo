//=============================================================================
//  Filename:   UIInteractiveListItem.cpp
//	---------------------------------------------------------------------------
//  Representation of list item with many interactive fields
//=============================================================================

#include "StdAfx.h"
#include "UIInteractiveListItem.h"
#include "../HUDManager.h"

//=============================================================================
//  CUIInteractiveListItem class
//=============================================================================
CUIInteractiveListItem::CUIInteractiveListItem()
	: itCurrIItem(vPositions.end()),
m_bInteractiveBahaviour(true)
{

}

//-----------------------------------------------------------------------------/
//  str:	������� ������, ��� ��� ������������
//	Data:	������ ���������� �� ������������� ��������
//	IDs:	� ���� ������� ������������� ���������� �������������� ��� ��������.
//			���������
//	height:	������ ��������
//	StartShift:	�������� ������������ ������ ���� � ��������
//-----------------------------------------------------------------------------/
void CUIInteractiveListItem::Init(const char *str, const xr_vector<char *> &Data, xr_vector<int> &IDs, float height)
{
	CGameFont	*pFont	= GetFont();
	R_ASSERT(pFont);

	// Counter
	int counter = 0;
	// C������� � ��������.
	u32 shift = 0;

	xr_string	strTmp;
	// ��������� � �������� ���������� � �������� �������� �������������� �����
	FIELDS_COORDS_VECTOR::value_type tmpPairs;

	// � ��� ��������� �� ��������, ������ ��� ��������, ��� � ������� ���������� ������ ���������� 
	// ������.
	R_ASSERT(Data.size() % 2 == 0);

	SetText(str);

	// C������ ��������� �����

	for (xr_vector<char *>::const_iterator it = Data.begin(); it != Data.end(); ++it, ++it)
	{
		// ��������� �� ���������
		strTmp.assign(*it, *(it + 1));
		tmpPairs.subStr = strTmp;
		// �������� ����������
		strTmp.assign(str, (*it));
		tmpPairs.pairScreenCrd.first = static_cast<int>(pFont->SizeOf(strTmp.c_str())) + shift;
		strTmp.assign(str, (*(it + 1)));
		tmpPairs.pairScreenCrd.second = static_cast<int>(pFont->SizeOf(strTmp.c_str())) + shift;
		// ID
		tmpPairs.ID = IDs[counter];
		// Save current
		vPositions.push_back(tmpPairs);
		++counter;
	}
	itCurrIItem = vPositions.end();
}

//-----------------------------------------------------------------------------/
//  ��������� ������� ����
//-----------------------------------------------------------------------------/
bool CUIInteractiveListItem::OnMouse(float x, float y, EUIMessages mouse_action)
{
	std::pair<float, float> tmpPair(x, y);
	itCurrIItem = std::find_if(vPositions.begin(), vPositions.end(), 
		std::bind2nd(mouse_hit(), tmpPair));

	// ������� �� ����� ������������� ������� ��������� ������ �� ����� �������, 
	// � ��������� �� ���� ��������
	if (m_bInteractiveBahaviour && WINDOW_LBUTTON_DOWN == mouse_action && itCurrIItem != vPositions.end())
	{
		GetMessageTarget()->SendMessage(this, INTERACTIVE_ITEM_CLICK, (void*)&(*itCurrIItem).ID);
		return	true;
	}
	else
		return inherited::OnMouse(x, y, mouse_action);
}

//-----------------------------------------------------------------------------/
//  �������� ���������� �������������� ���� ��� ���������
//-----------------------------------------------------------------------------/
Frect CUIInteractiveListItem::GetAbsoluteSubRect()
{
	Frect tmpRect;
	GetAbsoluteRect(tmpRect);
	if (itCurrIItem != vPositions.end())
	{
		tmpRect.left	+= (*itCurrIItem).pairScreenCrd.first;
		tmpRect.right	=  tmpRect.left + (*itCurrIItem).pairScreenCrd.second;
	}
	return tmpRect;
}

void CUIInteractiveListItem::Update()
{
	m_bHighlightText = false;
	inherited::Update();
}

void CUIInteractiveListItem::SetIItemID(const u32 uIndex, const int ID)
{
	R_ASSERT(vPositions.size() > uIndex);
	vPositions[uIndex].ID = ID;
}

u32 CUIInteractiveListItem::GetIFieldsCount()
{
	return vPositions.size();
}

//////////////////////////////////////////////////////////////////////////

void CUIInteractiveListItem::Draw()
{
	inherited::Draw();



	// ������������ ������������� �������
	if (itCurrIItem != vPositions.end() && m_bCursorOverWindow && m_bInteractiveBahaviour)
	{
		GetFont()->SetAligment(GetTextAlignment());

		GetFont()->SetColor(m_HighlightColor);
		Frect rect = GetSelfClipRect();
		CGameFont* F = GetFont();

		Fvector2					pos;
		pos.set						(rect.left + m_iTextOffsetX, rect.top + m_iTextOffsetY);
		UI()->ClientToScreenScaled	(pos);

		F->Out(	pos.x + 0, 
				pos.y + 0,
				(*itCurrIItem).subStr.c_str());

		F->Out(	pos.x + 0, 
				pos.y - 1,
				(*itCurrIItem).subStr.c_str());

		F->Out(pos.x + 1, 
				pos.y - 1,
				(*itCurrIItem).subStr.c_str());

		F->Out(	pos.x + 1, 
				pos.y + 0,
				(*itCurrIItem).subStr.c_str());

		F->Out(	pos.x + 1, 
				pos.y - 1,
				(*itCurrIItem).subStr.c_str());

		F->Out(	pos.x + 0, 
				pos.y + 1,
				(*itCurrIItem).subStr.c_str());

		F->Out(	pos.x - 1, 
				pos.y + 1,
				(*itCurrIItem).subStr.c_str());
		
		F->Out(	pos.x - 1, 
				pos.y + 0,
				(*itCurrIItem).subStr.c_str());

		F->Out(	pos.x - 1, 
				pos.y - 1,
				(*itCurrIItem).subStr.c_str());

	}
	// ����� ���� ������

	GetFont()->OnRender();
}