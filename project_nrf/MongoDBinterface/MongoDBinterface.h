
#include <iostream> 
#include <fstream> 
#include <mongocxx/client.hpp> 
#include <mongocxx/instance.hpp> 
#include <bsoncxx/builder/basic/document.hpp> 
#include <bsoncxx/builder/stream/document.hpp> 
#include <mongocxx/bulk_write.hpp>
#include <mongocxx/uri.hpp> 
#include <bsoncxx/json.hpp> 
#include <bsoncxx/types.hpp>
#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/document/view_or_value.hpp>

#include <string> 
 

class MongoDBConnector {
public:
    MongoDBConnector(const char* uri_string) {

        client_ = mongocxx::client{ mongocxx::uri{uri_string} };
        
    };


    void Create(const char* database_name, const char* collection_name) {

       // mongocxx::instance instance{};

        try {

            client_ = mongocxx::client{ mongocxx::uri{"mongodb://localhost:27017"} };
            mongocxx::database db = client_[database_name];

            // Создаем коллекцию 
            mongocxx::collection coll = db[collection_name];

            bsoncxx::document::value doc_value = bsoncxx::from_json("{\"key\": \"value\"}");
            coll.insert_one(doc_value.view());
            std::cout << std::endl<<"DB created!" << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

    }

    bool Drop(const char* database_name)
 {
    
     client_ = mongocxx::client{ mongocxx::uri{"mongodb://localhost:27017"} };
     try {
         client_.database(database_name).drop(); // Удаляем базу данных
         std::cout<<std::endl << "Database " << database_name << " deleted!" << std::endl;
         return true; // Успешно удалено
     }
     catch (const std::exception& e) {
         std::cerr << "Error whe deleting: " << e.what() << std::endl;
         return false; // Ошибка при удалении
     }
    

 }


    void InsertDocument(const char* database_name, const char* collection_name, const bsoncxx::document::value& document) {

        //mongocxx::instance instance{};
        auto db = client_[database_name];
        auto collection = db[collection_name];
        collection.insert_one(document.view());
    }

   
    void UpdateDocument(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter, const bsoncxx::document::view_or_value& update) {
        
        
        //mongocxx::instance instance{};
        
        auto db = client_[database_name]; 
        auto collection = db[collection_name];
        collection.update_one(filter, update);
    }

     void DeleteDocument(const char* database_name, const char* collection_name, const bsoncxx::document::view_or_value& filter) {

     //mongocxx::instance instance{};

     auto db = client_[database_name];
     auto collection = db[collection_name];
     collection.delete_one(filter);

     bsoncxx::stdx::optional<bsoncxx::document::value> result = collection.find_one(filter);

     if (result) {
         std::cout << "Doc not deleted." << std::endl;
         //std::cout << bsoncxx::to_json(result.value()) << std::endl; // Вывести найденный документ
     }
     else {
         std::cout << "Doc deleted." << std::endl;
     }
 }





private:
    mongocxx::client client_;
    mongocxx::uri uri;
    std::string dbname;
    std::string colname;
};

