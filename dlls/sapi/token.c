/*
 * Speech API (SAPI) token implementation.
 *
 * Copyright (C) 2017 Huw Davies
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "config.h"
#include <stdarg.h>

#define COBJMACROS

#include "windef.h"
#include "winbase.h"
#include "objbase.h"
#include "sapiddk.h"
#include "sperror.h"

#include "wine/debug.h"

#include "sapi_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(sapi);

struct data_key
{
    ISpRegDataKey ISpRegDataKey_iface;
    LONG ref;

    HKEY key;
    BOOL read_only;
};

struct data_key *impl_from_ISpRegDataKey( ISpRegDataKey *iface )
{
    return CONTAINING_RECORD( iface, struct data_key, ISpRegDataKey_iface );
}

static HRESULT WINAPI data_key_QueryInterface( ISpRegDataKey *iface, REFIID iid, void **obj )
{
    struct data_key *This = impl_from_ISpRegDataKey( iface );

    TRACE( "(%p)->(%s %p)\n", This, debugstr_guid( iid ), obj );

    if (IsEqualIID( iid, &IID_IUnknown ) ||
        IsEqualIID( iid, &IID_ISpDataKey ) ||
        IsEqualIID( iid, &IID_ISpRegDataKey ))
    {
        ISpRegDataKey_AddRef( iface );
        *obj = iface;
        return S_OK;
    }

    FIXME( "interface %s not implemented\n", debugstr_guid( iid ) );
    *obj = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI data_key_AddRef( ISpRegDataKey *iface )
{
    struct data_key *This = impl_from_ISpRegDataKey( iface );
    ULONG ref = InterlockedIncrement( &This->ref );

    TRACE( "(%p) ref = %u\n", This, ref );
    return ref;
}

static ULONG WINAPI data_key_Release( ISpRegDataKey *iface )
{
    struct data_key *This = impl_from_ISpRegDataKey( iface );
    ULONG ref = InterlockedDecrement(&This->ref);

    TRACE( "(%p) ref = %u\n", This, ref );

    if (!ref)
    {
        if (This->key) RegCloseKey( This->key );
        heap_free( This );
    }

    return ref;
}

static HRESULT WINAPI data_key_SetData( ISpRegDataKey *iface, LPCWSTR name,
                                        ULONG size, const BYTE *data )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_GetData( ISpRegDataKey *iface, LPCWSTR name,
                                        ULONG *size, BYTE *data )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_SetStringValue( ISpRegDataKey *iface,
                                               LPCWSTR name, LPCWSTR value )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_GetStringValue( ISpRegDataKey *iface,
                                               LPCWSTR name, LPWSTR *value )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_SetDWORD( ISpRegDataKey *iface,
                                         LPCWSTR name, DWORD value )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_GetDWORD( ISpRegDataKey *iface,
                                         LPCWSTR name, DWORD *pdwValue )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_OpenKey( ISpRegDataKey *iface,
                                        LPCWSTR name, ISpDataKey **sub_key )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_CreateKey( ISpRegDataKey *iface,
                                          LPCWSTR name, ISpDataKey **sub_key )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_DeleteKey( ISpRegDataKey *iface, LPCWSTR name )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_DeleteValue( ISpRegDataKey *iface, LPCWSTR name )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_EnumKeys( ISpRegDataKey *iface,
                                         ULONG index, LPWSTR *sub_key )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_EnumValues( ISpRegDataKey *iface,
                                           ULONG index, LPWSTR *value )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI data_key_SetKey( ISpRegDataKey *iface,
                                       HKEY key, BOOL read_only )
{
    struct data_key *This = impl_from_ISpRegDataKey( iface );

    TRACE( "(%p)->(%p %d)\n", This, key, read_only );

    if (This->key) return SPERR_ALREADY_INITIALIZED;

    This->key = key;
    This->read_only = read_only;
    return S_OK;
}

const struct ISpRegDataKeyVtbl data_key_vtbl =
{
    data_key_QueryInterface,
    data_key_AddRef,
    data_key_Release,
    data_key_SetData,
    data_key_GetData,
    data_key_SetStringValue,
    data_key_GetStringValue,
    data_key_SetDWORD,
    data_key_GetDWORD,
    data_key_OpenKey,
    data_key_CreateKey,
    data_key_DeleteKey,
    data_key_DeleteValue,
    data_key_EnumKeys,
    data_key_EnumValues,
    data_key_SetKey
};

HRESULT data_key_create( IUnknown *outer, REFIID iid, void **obj )
{
    struct data_key *This = heap_alloc( sizeof(*This) );
    HRESULT hr;

    if (!This) return E_OUTOFMEMORY;
    This->ISpRegDataKey_iface.lpVtbl = &data_key_vtbl;
    This->ref = 1;
    This->key = NULL;
    This->read_only = FALSE;

    hr = ISpRegDataKey_QueryInterface( &This->ISpRegDataKey_iface, iid, obj );

    ISpRegDataKey_Release( &This->ISpRegDataKey_iface );
    return hr;
}

struct token_category
{
    ISpObjectTokenCategory ISpObjectTokenCategory_iface;
    LONG ref;

    ISpRegDataKey *data_key;
};

struct token_category *impl_from_ISpObjectTokenCategory( ISpObjectTokenCategory *iface )
{
    return CONTAINING_RECORD( iface, struct token_category, ISpObjectTokenCategory_iface );
}

static HRESULT WINAPI token_category_QueryInterface( ISpObjectTokenCategory *iface,
                                                     REFIID iid, void **obj )
{
    struct token_category *This = impl_from_ISpObjectTokenCategory( iface );

    TRACE( "(%p)->(%s %p)\n", This, debugstr_guid( iid ), obj );

    if (IsEqualIID( iid, &IID_IUnknown ) ||
        IsEqualIID( iid, &IID_ISpDataKey ) ||
        IsEqualIID( iid, &IID_ISpObjectTokenCategory ))
    {
        ISpObjectTokenCategory_AddRef( iface );
        *obj = iface;
        return S_OK;
    }

    FIXME( "interface %s not implemented\n", debugstr_guid( iid ) );
    *obj = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI token_category_AddRef( ISpObjectTokenCategory *iface )
{
    struct token_category *This = impl_from_ISpObjectTokenCategory( iface );
    ULONG ref = InterlockedIncrement( &This->ref );

    TRACE( "(%p) ref = %u\n", This, ref );
    return ref;
}

static ULONG WINAPI token_category_Release( ISpObjectTokenCategory *iface )
{
    struct token_category *This = impl_from_ISpObjectTokenCategory( iface );
    ULONG ref = InterlockedDecrement(&This->ref);

    TRACE( "(%p) ref = %u\n", This, ref );

    if (!ref)
    {
        if (This->data_key) ISpRegDataKey_Release( This->data_key );
        heap_free( This );
    }
    return ref;
}

static HRESULT WINAPI token_category_SetData( ISpObjectTokenCategory *iface,
                                              LPCWSTR name, ULONG size,
                                              const BYTE *data )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_GetData( ISpObjectTokenCategory *iface,
                                              LPCWSTR name, ULONG *size,
                                              BYTE *data )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_SetStringValue( ISpObjectTokenCategory *iface,
                                                     LPCWSTR name, LPCWSTR value )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_GetStringValue( ISpObjectTokenCategory *iface,
                                                     LPCWSTR name, LPWSTR *value )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_SetDWORD( ISpObjectTokenCategory *iface,
                                               LPCWSTR name, DWORD value )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_GetDWORD( ISpObjectTokenCategory *iface,
                                               LPCWSTR name, DWORD *pdwValue )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_OpenKey( ISpObjectTokenCategory *iface,
                                              LPCWSTR name, ISpDataKey **sub_key )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_CreateKey( ISpObjectTokenCategory *iface,
                                                LPCWSTR name, ISpDataKey **sub_key )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_DeleteKey( ISpObjectTokenCategory *iface,
                                                LPCWSTR name )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_DeleteValue( ISpObjectTokenCategory *iface,
                                                  LPCWSTR name )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_EnumKeys( ISpObjectTokenCategory *iface,
                                               ULONG index, LPWSTR *sub_key )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_EnumValues( ISpObjectTokenCategory *iface,
                                                 ULONG index, LPWSTR *value )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT parse_cat_id( const WCHAR *str, HKEY *root, const WCHAR **sub_key )
{
    static const WCHAR HKLM[] = {'H','K','E','Y','_','L','O','C','A','L','_','M','A','C','H','I','N','E','\\'};
    static const WCHAR HKCU[] = {'H','K','E','Y','_','C','U','R','R','E','N','T','_','U','S','E','R','\\'};
    struct table
    {
        const WCHAR *name;
        int size;
        HKEY key;
    } table[] =
    {
        { HKLM, sizeof(HKLM), HKEY_LOCAL_MACHINE },
        { HKCU, sizeof(HKCU), HKEY_CURRENT_USER },
        { NULL }
    };
    struct table *ptr;
    int len = strlenW( str );

    for (ptr = table; ptr->name; ptr++)
    {
        if (len >= ptr->size / sizeof(ptr->name[0]) && !memcmp( str, ptr->name, ptr->size ))
        {
            *root = ptr->key;
            *sub_key = str + ptr->size / sizeof(ptr->name[0]);
            return S_OK;
        }
    }
    return S_FALSE;
}

static HRESULT WINAPI token_category_SetId( ISpObjectTokenCategory *iface,
                                            LPCWSTR id, BOOL create )
{
    struct token_category *This = impl_from_ISpObjectTokenCategory( iface );
    HKEY root, key;
    const WCHAR *subkey;
    LONG res;
    HRESULT hr;

    TRACE( "(%p)->(%s %d)\n", This, debugstr_w( id ), create );

    if (This->data_key) return SPERR_ALREADY_INITIALIZED;

    hr = parse_cat_id( id, &root, &subkey );
    if (hr != S_OK) return SPERR_INVALID_REGISTRY_KEY;

    if (create) FIXME( "Ignoring create\n" );

    res = RegOpenKeyExW( root, subkey, 0, KEY_ALL_ACCESS, &key );
    if (res) return SPERR_INVALID_REGISTRY_KEY;

    hr = CoCreateInstance( &CLSID_SpDataKey, NULL, CLSCTX_ALL,
                           &IID_ISpRegDataKey, (void **)&This->data_key );
    if (FAILED(hr)) goto fail;

    hr = ISpRegDataKey_SetKey( This->data_key, key, FALSE );
    if (FAILED(hr)) goto fail;

    return hr;

fail:
    RegCloseKey( key );
    return hr;
}

static HRESULT WINAPI token_category_GetId( ISpObjectTokenCategory *iface,
                                            LPWSTR *id )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_GetDataKey( ISpObjectTokenCategory *iface,
                                                 SPDATAKEYLOCATION location,
                                                 ISpDataKey **data_key )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_EnumTokens( ISpObjectTokenCategory *iface,
                                                 LPCWSTR req, LPCWSTR opt,
                                                 IEnumSpObjectTokens **enum_tokens )
{
    struct token_category *This = impl_from_ISpObjectTokenCategory( iface );
    ISpObjectTokenEnumBuilder *builder;
    HRESULT hr;

    FIXME( "(%p)->(%s %s %p): semi-stub\n", This, debugstr_w( req ), debugstr_w( opt ), enum_tokens );

    if (!This->data_key) return SPERR_UNINITIALIZED;

    hr = CoCreateInstance( &CLSID_SpObjectTokenEnum, NULL, CLSCTX_ALL,
                           &IID_ISpObjectTokenEnumBuilder, (void **)&builder );
    if (FAILED(hr)) return hr;

    hr = ISpObjectTokenEnumBuilder_SetAttribs( builder, req, opt );
    if (FAILED(hr)) goto fail;

    /* FIXME: Build the enumerator */

    hr = ISpObjectTokenEnumBuilder_QueryInterface( builder, &IID_IEnumSpObjectTokens,
                                                   (void **)enum_tokens );

fail:
    ISpObjectTokenEnumBuilder_Release( builder );
    return hr;
}

static HRESULT WINAPI token_category_SetDefaultTokenId( ISpObjectTokenCategory *iface,
                                                        LPCWSTR id )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_category_GetDefaultTokenId( ISpObjectTokenCategory *iface,
                                                        LPWSTR *id )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

const struct ISpObjectTokenCategoryVtbl token_category_vtbl =
{
    token_category_QueryInterface,
    token_category_AddRef,
    token_category_Release,
    token_category_SetData,
    token_category_GetData,
    token_category_SetStringValue,
    token_category_GetStringValue,
    token_category_SetDWORD,
    token_category_GetDWORD,
    token_category_OpenKey,
    token_category_CreateKey,
    token_category_DeleteKey,
    token_category_DeleteValue,
    token_category_EnumKeys,
    token_category_EnumValues,
    token_category_SetId,
    token_category_GetId,
    token_category_GetDataKey,
    token_category_EnumTokens,
    token_category_SetDefaultTokenId,
    token_category_GetDefaultTokenId,
};

HRESULT token_category_create( IUnknown *outer, REFIID iid, void **obj )
{
    struct token_category *This = heap_alloc( sizeof(*This) );
    HRESULT hr;

    if (!This) return E_OUTOFMEMORY;
    This->ISpObjectTokenCategory_iface.lpVtbl = &token_category_vtbl;
    This->ref = 1;
    This->data_key = NULL;

    hr = ISpObjectTokenCategory_QueryInterface( &This->ISpObjectTokenCategory_iface, iid, obj );

    ISpObjectTokenCategory_Release( &This->ISpObjectTokenCategory_iface );
    return hr;
}

struct token_enum
{
    ISpObjectTokenEnumBuilder ISpObjectTokenEnumBuilder_iface;
    LONG ref;

    BOOL init;
    WCHAR *req, *opt;
    ULONG count;
};

struct token_enum *impl_from_ISpObjectTokenEnumBuilder( ISpObjectTokenEnumBuilder *iface )
{
    return CONTAINING_RECORD( iface, struct token_enum, ISpObjectTokenEnumBuilder_iface );
}

static HRESULT WINAPI token_enum_QueryInterface( ISpObjectTokenEnumBuilder *iface,
                                                 REFIID iid, void **obj )
{
    struct token_enum *This = impl_from_ISpObjectTokenEnumBuilder( iface );

    TRACE( "(%p)->(%s %p)\n", This, debugstr_guid( iid ), obj );

    if (IsEqualIID( iid, &IID_IUnknown ) ||
        IsEqualIID( iid, &IID_IEnumSpObjectTokens ) ||
        IsEqualIID( iid, &IID_ISpObjectTokenEnumBuilder ))
    {
        ISpObjectTokenEnumBuilder_AddRef( iface );
        *obj = iface;
        return S_OK;
    }

    FIXME( "interface %s not implemented\n", debugstr_guid( iid ) );
    *obj = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI token_enum_AddRef( ISpObjectTokenEnumBuilder *iface )
{
    struct token_enum *This = impl_from_ISpObjectTokenEnumBuilder( iface );
    ULONG ref = InterlockedIncrement( &This->ref );

    TRACE( "(%p) ref = %u\n", This, ref );
    return ref;
}

static ULONG WINAPI token_enum_Release( ISpObjectTokenEnumBuilder *iface )
{
    struct token_enum *This = impl_from_ISpObjectTokenEnumBuilder( iface );
    ULONG ref = InterlockedDecrement(&This->ref);

    TRACE( "(%p) ref = %u\n", This, ref );

    if (!ref)
    {
        heap_free( This->req );
        heap_free( This->opt );
        heap_free( This );
    }

    return ref;
}

static HRESULT WINAPI token_enum_Next( ISpObjectTokenEnumBuilder *iface,
                                       ULONG num, ISpObjectToken **tokens,
                                       ULONG *fetched )
{
    struct token_enum *This = impl_from_ISpObjectTokenEnumBuilder( iface );

    TRACE( "(%p)->(%u %p %p)\n", This, num, tokens, fetched );

    if (!This->init) return SPERR_UNINITIALIZED;

    FIXME( "semi-stub: Returning an empty enumerator\n" );

    if (fetched) *fetched = 0;
    return S_FALSE;
}

static HRESULT WINAPI token_enum_Skip( ISpObjectTokenEnumBuilder *iface,
                                       ULONG num )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_enum_Reset( ISpObjectTokenEnumBuilder *iface)
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_enum_Clone( ISpObjectTokenEnumBuilder *iface,
                                        IEnumSpObjectTokens **clone )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_enum_Item( ISpObjectTokenEnumBuilder *iface,
                                       ULONG index, ISpObjectToken **token )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_enum_GetCount( ISpObjectTokenEnumBuilder *iface,
                                           ULONG *count )
{
    struct token_enum *This = impl_from_ISpObjectTokenEnumBuilder( iface );

    TRACE( "(%p)->(%p)\n", This, count );

    if (!This->init) return SPERR_UNINITIALIZED;

    *count = This->count;
    return S_OK;
}

static HRESULT WINAPI token_enum_SetAttribs( ISpObjectTokenEnumBuilder *iface,
                                             LPCWSTR req, LPCWSTR opt)
{
    struct token_enum *This = impl_from_ISpObjectTokenEnumBuilder( iface );

    TRACE( "(%p)->(%s %s)\n", This, debugstr_w( req ), debugstr_w( opt ) );

    if (This->init) return SPERR_ALREADY_INITIALIZED;

    if (req)
    {
        This->req = heap_strdupW( req );
        if (!This->req) goto out_of_mem;
    }

    if (opt)
    {
        This->opt = heap_strdupW( opt );
        if (!This->opt) goto out_of_mem;
    }

    This->init = TRUE;
    return S_OK;

out_of_mem:
    heap_free( This->req );
    return E_OUTOFMEMORY;
}

static HRESULT WINAPI token_enum_AddTokens( ISpObjectTokenEnumBuilder *iface,
                                            ULONG num, ISpObjectToken **tokens )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_enum_AddTokensFromDataKey( ISpObjectTokenEnumBuilder *iface,
                                                       ISpDataKey *data_key,
                                                       LPCWSTR sub_key, LPCWSTR cat_id )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_enum_AddTokensFromTokenEnum( ISpObjectTokenEnumBuilder *iface,
                                                         IEnumSpObjectTokens *token_enum )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

static HRESULT WINAPI token_enum_Sort( ISpObjectTokenEnumBuilder *iface,
                                       LPCWSTR first )
{
    FIXME( "stub\n" );
    return E_NOTIMPL;
}

const struct ISpObjectTokenEnumBuilderVtbl token_enum_vtbl =
{
    token_enum_QueryInterface,
    token_enum_AddRef,
    token_enum_Release,
    token_enum_Next,
    token_enum_Skip,
    token_enum_Reset,
    token_enum_Clone,
    token_enum_Item,
    token_enum_GetCount,
    token_enum_SetAttribs,
    token_enum_AddTokens,
    token_enum_AddTokensFromDataKey,
    token_enum_AddTokensFromTokenEnum,
    token_enum_Sort
};

HRESULT token_enum_create( IUnknown *outer, REFIID iid, void **obj )
{
    struct token_enum *This = heap_alloc( sizeof(*This) );
    HRESULT hr;

    if (!This) return E_OUTOFMEMORY;
    This->ISpObjectTokenEnumBuilder_iface.lpVtbl = &token_enum_vtbl;
    This->ref = 1;
    This->req = NULL;
    This->opt = NULL;
    This->init = FALSE;
    This->count = 0;

    hr = ISpObjectTokenEnumBuilder_QueryInterface( &This->ISpObjectTokenEnumBuilder_iface, iid, obj );

    ISpObjectTokenEnumBuilder_Release( &This->ISpObjectTokenEnumBuilder_iface );
    return hr;
}
