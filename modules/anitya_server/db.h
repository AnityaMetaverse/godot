#ifndef DB_H
#define DB_H

#include "core/reference.h"


class LocalAssetData: public Reference
{
    GDCLASS(LocalAssetData, Reference);

    private:
        int id;
        String asset_name;
        String extension;
        PoolByteArray data;
    
    protected:
        static void _bind_methods();
    
    public:
        void set_asset_name(const String& p_asset_name) { asset_name = p_asset_name; }
        String get_asset_name() const { return asset_name; }

        void set_extension(const String& p_extension) { extension = p_extension; }
        String get_extension() const { return extension; }

        void set_data(const PoolByteArray& p_data) { data = p_data; }
        PoolByteArray get_data() const { return data; }


        
};



class IDB: public Reference
{
    GDCLASS(IDB, Reference);

    protected:
        static void _bind_methods();

    public:
        enum DBError
        {
            DB_OK = OK,
            DB_MISSING_PARAM,
            DB_UNKOWN
        };

        virtual void init() {}
        virtual Ref<LocalAssetData> get_asset(int p_asset_id) { return nullptr; }
        virtual int create_asset(Ref<LocalAssetData> p_data) { return 0; }
};

#endif