#include <iostream>
#include <string>
#include <fstream>
#include <mysql/jdbc.h>

using namespace std;

// Clase base para el conector de base de datos
class DatabaseConnector { // Clase
public:
    virtual void insert(string query) = 0; // Métodos
    virtual void update(string query) = 0;
    virtual void del(string query) = 0;
    virtual void select(string query) = 0;
    virtual string getSelectedTable() = 0;
    virtual void setSelectedTable(string tableName) = 0;
    virtual void saveToFile(string filename) = 0;
    virtual void loadFromFile(string filename) = 0;
};

// Clase derivada específica para MySQL
class MySQLConnector : public DatabaseConnector { // Clase y Herencia de Database Connector
private:
    sql::Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::ResultSet* res;
    string selectedTable; // Función para seleccionar tabla

public:
    MySQLConnector(string host, string user, string password, string database) { // Costructor
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

    ~MySQLConnector() { // Destructor
        delete res;
        delete stmt;
        delete con;
    }

    void setSelectedTable(string tableName) override { // Método
        selectedTable = tableName;
    }

    void insert(string query) override { // Método
        try {
            stmt->execute(query);
            cout << "Registro insertado correctamente." << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "Error al insertar registro: " << e.what() << endl;
        }
    }

    void update(string query) override { // Método
        try {
            stmt->execute(query);
            cout << "Registro actualizado correctamente." << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "Error al actualizar registro: " << e.what() << endl;
        }
    }

    void del(string query) override { // Método
        try {
            stmt->execute(query);
            cout << "Registro eliminado correctamente." << endl;
        }
        catch (sql::SQLException& e) {
            cerr << "Error al eliminar registro: " << e.what() << endl;
        }
    }

    void select(string query) override { // Método
        try {
            res = stmt->executeQuery(query);

            // Verificar si la tabla seleccionada es "clientes"
            if (selectedTable == "clientes") { // Condicional
                while (res->next()) {
                    cout << "ID: " << res->getInt("id") << ", Nombre: " << res->getString("nombre") << ", Telefono: " << res->getInt("telefono") << endl;
                }
            }
            // Verificar si la tabla seleccionada es "juegos"
            else if (selectedTable == "juegos") { // Condicional
                while (res->next()) {
                    cout << "ID: " << res->getInt("id") << ", Nombre del juego: " << res->getString("nombre") << ", Precio: " << res->getDouble("precio") << endl;
                }
            }
            // verifica si la tabla seleccionada es "reservas"
            else if (selectedTable == "reservas") { //Condicional
                while (res->next()) {
                    cout << "ID: " << res->getInt("id") << ", Id del cliente: " << res->getInt("cliente_id") << ", Fecha y hora: " << res->getString("fecha_hora") << ", Total: " << res->getDouble("total") << endl;
                }
            }
            // En caso de que la tabla seleccionada no sea ni "clientes" ni "juegos" ni "reservas"
            else {
                cerr << "Error: No se pudo determinar la tabla seleccionada." << endl;
            }
        }
        catch (sql::SQLException& e) {
            cerr << "Error al seleccionar registros: " << e.what() << endl;
        }
    }

    string getSelectedTable() override { // Método
        return selectedTable;
    }

    void saveToFile(string filename) override { // Método
        ofstream file(filename);// Archivo de salida
        if (!file.is_open()) { // Condicional
            cerr << "No se pudo abrir el archivo para guardar." << endl;
            return;
        }

        res = stmt->executeQuery("SELECT * FROM " + selectedTable);

        if (selectedTable == "clientes") { // Condicional
            while (res->next()) {
                file << res->getInt("id") << "," << res->getString("nombre") << "," << res->getInt("telefono") << endl;
            }
        }
        else if (selectedTable == "juegos") { // Condicional
            while (res->next()) {
                file << res->getInt("id") << "," << res->getString("nombre") << "," << res->getDouble("precio") << endl;
            }
        }
        else if (selectedTable == "reservas") { // Condicional
            while (res->next()) {
                file << res->getInt("id") << "," << res->getInt("cliente_id") << "," << res->getString("fecha_hora") << "," << res->getDouble("total") << endl;
            }
        }
        else { 
            cerr << "Tabla no reconocida." << endl; // Condicional
        }

        file.close();
        cout << "Datos guardados en " << filename << endl;
    }

    void loadFromFile(string filename) override { // Método
        ifstream file(filename);// Archivo de entrada
        if (!file.is_open()) {// Condicional
            cerr << "No se pudo abrir el archivo para cargar." << endl;
            return;
        }

        string line;
        while (getline(file, line)) { // Ciclo
            cout << line << endl;
        }

        file.close(); // Cierre de archivo
        cout << "Datos cargados desde " << filename << endl;
    }
};

// Función para seleccionar la tabla
string selectTable() {
    string selectedTable;
    system("color 8e");
    cout << "Seleccione la tabla:" << endl;
    cout << "1. Clientes" << endl;
    cout << "2. Juegos" << endl;
    cout << "3. Reservas" << endl;
    cout << "Opcion: ";
    int option;
    cin >> option;
    switch (option) {
    case 1:
        selectedTable = "clientes";
        break;
    case 2:
        selectedTable = "juegos";
        break;
    case 3:
        selectedTable = "reservas";
        break;
    default:
        cerr << "Opcion no valida." << endl;
        exit(1);
    }
    return selectedTable;
}

// Función que utiliza polimorfismo para operar sobre DatabaseConnector
void performDatabaseOperations(DatabaseConnector* connector) { // Función
    int opcion;
    string query;
    system("color 8e");

    do { // Ciclo
        system("cls");
        cout << "---------Menu----------" << endl;
        cout << "1. Insertar registro" << endl;
        cout << "2. Actualizar registro" << endl;
        cout << "3. Eliminar registro" << endl;
        cout << "4. Mostrar registros" << endl;
        cout << "5. Guardar registros en archivo" << endl;
        cout << "6. Cargar registros desde archivo" << endl;
        cout << "7. Seleccionar otra tabla" << endl;
        cout << "8. Salir" << endl;
        cout << "-----------------------" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();  // Para limpiar el buffer de entrada
        system("cls");

        switch (opcion) {
        case 1:
            cout << "Ingrese el query de insercion (Ejemplo: INSERT INTO tabla (campo1, campo2) VALUES (valor1, valor2)): ";
            getline(cin, query);
            connector->insert(query);
            system("pause");
            break;
        case 2:
            cout << "Ingrese el query de actualizacion (Ejemplo: UPDATE tabla SET campo1 = valor1 WHERE condicion): ";
            getline(cin, query);
            connector->update(query);// Llamada a método
            system("pause");
            break;
        case 3:
            cout << "Ingrese el query de eliminacion (Ejemplo: DELETE FROM tabla WHERE condicion): ";
            getline(cin, query);
            connector->del(query);// Llamada a método
            system("pause");
            break;
        case 4:
            cout << "Mostrando registros:" << endl;
            connector->select("SELECT * FROM " + connector->getSelectedTable());// Llamada a método
            system("pause");
            break;
        case 5:
            cout << "Ingrese el nombre del archivo para guardar: ";
            getline(cin, query);
            connector->saveToFile(query);// Llamada a método
            system("pause");
            break;
        case 6:
            cout << "Ingrese el nombre del archivo para cargar: ";
            getline(cin, query);
            connector->loadFromFile(query);// Llamada a método
            system("pause");
            break;
        case 7:
            connector->setSelectedTable(selectTable());
            break;
        case 8:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opcion no valida. Por favor, intente de nuevo." << endl; // Condicional
        }
    } while (opcion != 8); // Condicional
}

int main() {
    MySQLConnector mySQLConnector("localhost", "root", "123456789", "black");
    mySQLConnector.setSelectedTable(selectTable()); // Llamada a Método
    performDatabaseOperations(&mySQLConnector); // Llamada a función
    return 0;
}