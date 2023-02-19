// File: mrulist.h

#ifndef _MRULIST2_H_
#define _MRULIST2_H_

#include "ConfUtil.h"
#include "richaddr.h"

typedef VOID * PMRUE;  // MRU Entries

enum {
	MRUTYPE_SZ = 1,
	MRUTYPE_DW = 2,
};

class CMRUList2
{
private:
	const DWSTR * m_prgDwStr;// {{cCol, pszKey}, {mruType, pszPrefix1}, {mruType, pszPrefix2},...}
	int     m_cCol;       // number of "columns" (data entries in m_prgDwStr)
	int     m_cEntryMax;  // maximum number of entries
	int	    m_cEntry;     // current number of entries
	PMRUE * m_rgpEntry;   // array of pointers to MRU data
	BOOL    m_fDirty;     // TRUE if data was changed
	BOOL    m_fReversed;  // Load/Save data reversed (old style)

	BOOL    FValidCol(int i)    {return ((i >= 0) && (i < m_cCol));}
	BOOL    FValidIndex(int i)  {return ((i >= 0) && (i < m_cEntry));}
	BOOL    FDirty()            {return m_fDirty;}
	BOOL    FReversed()         {return m_fReversed;}

	inline PMRUE GetEntry(int iItem)
	{
		ASSERT(FValidIndex(iItem));
		return m_rgpEntry[iItem];
	}

	inline int MruTypeForCol(int iCol)
	{
		ASSERT(FValidCol(iCol));
		return m_prgDwStr[1+iCol].dw;
	}

	inline LPCTSTR PszPrefixForCol(int iCol)
	{
		ASSERT(FValidCol(iCol));
		return m_prgDwStr[1+iCol].psz;
	}

	inline LPCTSTR PszRegKey(void)
	{
		return m_prgDwStr[0].psz;
	}

	VOID    ShiftEntriesDown(int cItem);

protected:
	virtual int CompareEntry(int iItem, PMRUE pEntry);

public:
	CMRUList2(const DWSTR * prgDwStr, int cEntryMax, BOOL fReverse = FALSE);
	~CMRUList2();

	int     GetNumEntries()     {return m_cEntry;}
	VOID    SetDirty(BOOL fDirty)     {m_fDirty = fDirty;}

	// Generic functions
	int     FindEntry(PMRUE pEntry);
	VOID    MoveEntryToTop(int iItem);
	PMRUE   LoadEntry(RegEntry * pre, int iItem);
	VOID    StoreEntry(RegEntry * pre, int iItem);
	VOID    DeleteEntry(PMRUE pEntry);
	VOID    DeleteEntry(int iItem);

	void
	DeleteEntry
	(
		const TCHAR * const	primaryString
	);
	 
	HRESULT Save(void);

	LPCTSTR GetString(int iItem, int iCol);
	DWORD GetDWORD(int iItem, int iCol);

	LPCTSTR GetString(PMRUE pEntry, int iCol);
	DWORD GetDWORD(PMRUE pEntry, int iCol);

	HRESULT AddEntry(PMRUE pEntry);
	HRESULT AddEntry(LPCTSTR pcsz);
	HRESULT AddEntry(LPCTSTR pcsz1, LPCTSTR pcsz2);
	HRESULT AddEntry(LPCTSTR pcsz1, LPCTSTR pcsz2, DWORD dw3);
};


#endif /* _MRULIST2_H_ */

