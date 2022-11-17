#include "db.h"


void LocalAssetData::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_asset_name", "asset_name"), &LocalAssetData::set_asset_name);
    ClassDB::bind_method(D_METHOD("get_asset_name"), &LocalAssetData::get_asset_name);

    ClassDB::bind_method(D_METHOD("set_extension", "extension"), &LocalAssetData::set_extension);
    ClassDB::bind_method(D_METHOD("get_extension"), &LocalAssetData::get_extension);

    ClassDB::bind_method(D_METHOD("set_data", "asset_data"), &LocalAssetData::set_data);
    ClassDB::bind_method(D_METHOD("get_data"), &LocalAssetData::get_data);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "asset_name"), "set_asset_name", "get_asset_name");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "extension"), "set_extension", "get_extension");
    ADD_PROPERTY(PropertyInfo(Variant::POOL_BYTE_ARRAY, "data"), "set_data", "get_data");
}

void IDB::_bind_methods()
{
    BIND_VMETHOD(MethodInfo(Variant::BOOL, "init", PropertyInfo(Variant::STRING, "path")));
    BIND_VMETHOD(MethodInfo(Variant::BOOL, "execute", PropertyInfo(Variant::STRING, "query")));
    BIND_VMETHOD(MethodInfo(Variant::BOOL, "execute_with_args", PropertyInfo(Variant::STRING, "query"), PropertyInfo(Variant::ARRAY, "args")));
    BIND_VMETHOD(MethodInfo(Variant::ARRAY, "fetch", PropertyInfo(Variant::STRING, "query")));
    BIND_VMETHOD(MethodInfo(Variant::ARRAY, "fetch_with_args", PropertyInfo(Variant::STRING, "query"), PropertyInfo(Variant::ARRAY, "args")));
    BIND_VMETHOD(MethodInfo(Variant::STRING, "get_error_message"));
}