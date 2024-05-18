#include <iostream>
#include <string>
#include <mysql/jdbc.h> // Incluir la cabecera de MySQL Connector/C++

using namespace std;

// Clase base para el conector de base de datos
class DatabaseConnector {
public:
    virtual void insert(string query) = 0;
    virtual void update(string query) = 0;
    virtual void del(string query) = 0;
    virtual void select(string query) = 0;
};

// Clase derivada específica para MySQL
class MySQLConnector : public DatabaseConnector {
private:
    sql::Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res;

public:
    MySQLConnector(string host, string user, string password, string database) {
        try {
            driver = get_driver_instance();
            con = driver->connect(host, user, password);
            con->setSchema(database);
            stmt = con->createStatement();
        }
        catch (sql::SQLException& e) {
            cerr << "Error al conectar a la base de datos: " << e.what() << endl;
            exit(1);
        }
    }

    ~MySQLConnector() {
        delete res;
        delete stmt;
        delete con;
    }

    void insert(string query) override {
        try {
            stmt->execute(query);
            cout << "Registro insertado correctamente." << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "Error al insertar registro: " << e.what() << endl;
        }
    }

    void update(string query) override {
        try {
            stmt->execute(query);
            cout << "Registro actualizado correctamente." << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "Error al actualizar registro: " << e.what() << endl;
        }
    }

    void del(string query) override {
        try {
            stmt->execute(query);
            cout << "Registro eliminado correctamente." << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "Error al eliminar registro: " << e.what() << endl;
        }
    }

    void select(string query) override {
        try {
            res = stmt->executeQuery(query);
            while (res->next()) {
                cout << "ID: " << res->getInt("id") << ", Nombre: " << res->getString("nombre") << ", Telefono: " << res->getInt("telefono") << endl;
            }
        }
        catch (sql::SQLException& e) {
            cerr << "Error al seleccionar registros: " << e.what() << endl;
        }
    }
};

int main() {
    MySQLConnector connector("localhost", "root", "123456789", "black");

    int opcion;
    string query;
    system("color 8e");

    do {
        system("cls");
        cout << "---------Menu----------" << endl;
        cout << "1. Insertar registro" << endl;
        cout << "2. Actualizar registro" << endl;
        cout << "3. Eliminar registro" << endl;
        cout << "4. Mostrar registros" << endl;
        cout << "5. Salir" << endl;
        cout << "-----------------------" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        system("cls");

        switch (opcion) {
        case 1:
            cout << "Ingrese el query de insercion (Ejemplo: INSERT INTO tabla (campo1, campo2) VALUES (valor1, valor2)): ";
            cin.ignore();
            getline(cin, query);
            connector.insert(query);
            system("pause");
            break;
        case 2:
            cout << "Ingrese el query de actualizacion (Ejemplo: UPDATE tabla SET campo1 = valor1 WHERE condicion): ";
            cin.ignore();
            getline(cin, query);
            connector.update(query);
            system("pause");
            break;
        case 3:
            cout << "Ingrese el query de eliminacion (Ejemplo: DELETE FROM tabla WHERE condicion): ";
            cin.ignore();
            getline(cin, query);
            connector.del(query);
            system("pause");
            break;
        case 4:
            cout << "Ingrese el query de seleccion (Ejemplo: SELECT * FROM tabla WHERE condicion): ";
            cin.ignore();
            getline(cin, query);
            connector.select(query);
            system("pause");
            break;
        case 5:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opcion no valida. Por favor, intente de nuevo." << endl;
        }
    } while (opcion != 5);

    return 0;
}