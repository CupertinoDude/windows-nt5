//////////////////////////////////////////////////////////////////////////////
/*++

Copyright (C) Microsoft Corporation, 1997 - 1999

Module Name:

    LogCompD.cpp

Abstract:

   The CLoggingComponentData class implements several interfaces which MMC uses:
   
   The IComponentData interface is basically how MMC talks to the snap-in
   to get it to implement the left-hand-side "scope" pane.

   The IExtendPropertySheet interface is how the snap-in adds property sheets
   for any of the items a user might click on.

   The IExtendContextMenu interface what we do to add custom entries
   to the menu which appears when a user right-clicks on a node.
   
   The IExtendControlBar interface allows us to support a custom
   iconic toolbar.

   See ComponentData.cpp for implementation.

Note:

   Much of the functionality of this class is implemented in atlsnap.h
   by IComponentDataImpl.  We are mostly overriding here.

Revision History:
   mmaguire 11/6/97 - created using MMC snap-in wizard


--*/
//////////////////////////////////////////////////////////////////////////////

#if !defined(_LOG_COMPONENT_DATA_H_)
#define _LOG_COMPONENT_DATA_H_

//////////////////////////////////////////////////////////////////////////////
// BEGIN INCLUDES
//
// where we can find what this class derives from:
//
//Moved to Precompiled.h: #include <atlsnap.h>
//
//
// where we can find what this class has or uses:
//
#include "LogMacNd.h"
//
// END INCLUDES
//////////////////////////////////////////////////////////////////////////////


   //
   // hack start
   //

   // This is a big hack to work around a atlsnap.h bug: Atlsnap.h
   // can't support extending multiple nodes. So we basically just
   // copied EXTENSION_SNAPIN_NODEINFO_ENTRY() here. We need to change
   // this after the atlsnap.h fix -- MAM: 08-06-98 -- yeah right
   //

   //
   // The following statements are copied from atlsnap.h and then changed
   // to support multiple extending node
   //
   // IsSupportedGUID will also set the m_enumExtendedSnapin flag in side m_##dataClass object
   // which, in our case, is CLoggingMachineNode
   //

#define EXTENSION_SNAPIN_NODEINFO_ENTRY_EX(dataClass) \
   if ( m_##dataClass.IsSupportedGUID( guid ) )\
   { \
      *ppItem = m_##dataClass.GetExtNodeObject(pDataObject, &m_##dataClass); \
      _ASSERTE(*ppItem != NULL); \
      (*ppItem)->InitDataClass(pDataObject, &m_##dataClass); \
      return hr; \
   }

   //
   // hack end
   //

class CLoggingComponent;

class CLoggingComponentData :
     public CComObjectRootEx<CComSingleThreadModel>
   , public CSnapInObjectRoot<1, CLoggingComponentData>
   , public IComponentDataImpl<CLoggingComponentData, CLoggingComponent>
   , public IExtendPropertySheetImpl<CLoggingComponentData>
   , public IExtendContextMenuImpl<CLoggingComponentData>
   , public IExtendControlbarImpl<CLoggingComponentData>
   , public ISnapinHelp
   , public CComCoClass<CLoggingComponentData, &CLSID_LoggingSnapin>
{

public:

   CLoggingComponentData();

   ~CLoggingComponentData();

   EXTENSION_SNAPIN_DATACLASS(CLoggingMachineNode)

   BEGIN_EXTENSION_SNAPIN_NODEINFO_MAP(CLoggingComponentData)
   EXTENSION_SNAPIN_NODEINFO_ENTRY_EX(CLoggingMachineNode)
   END_EXTENSION_SNAPIN_NODEINFO_MAP()

   BEGIN_COM_MAP(CLoggingComponentData)
      COM_INTERFACE_ENTRY(IComponentData)
      COM_INTERFACE_ENTRY(IExtendPropertySheet2)
      COM_INTERFACE_ENTRY(IExtendContextMenu)
      COM_INTERFACE_ENTRY(IExtendControlbar)
      COM_INTERFACE_ENTRY(ISnapinHelp)
   END_COM_MAP()

   DECLARE_REGISTRY_RESOURCEID(IDR_NAPSNAPIN)

   DECLARE_NOT_AGGREGATABLE(CLoggingComponentData)

    STDMETHOD(Initialize)(LPUNKNOWN pUnknown);

   STDMETHOD(CompareObjects)(
        LPDATAOBJECT lpDataObjectA
      , LPDATAOBJECT lpDataObjectB
      );

   STDMETHOD(CreateComponent)(LPCOMPONENT *ppComponent);

   // ISnapinHelp method(s)
   STDMETHOD(GetHelpTopic)(LPOLESTR * lpCompiledHelpFile)
   {return E_UNEXPECTED;};

   // We are overiding ATLsnap.h's IComponentImpl implementation of this
   // in order to correctly handle messages which it is incorrectly
   // ignoring (e.g. MMCN_COLUMN_CLICK and MMCN_SNAPINHELP)
   STDMETHOD(Notify)(
          LPDATAOBJECT lpDataObject
        , MMC_NOTIFY_TYPE event
        , LPARAM arg
        , LPARAM param
      );

   virtual HRESULT OnPropertyChange(   
        LPARAM arg
      , LPARAM param
      );
};

#endif // _LOG_COMPONENT_DATA_H_
