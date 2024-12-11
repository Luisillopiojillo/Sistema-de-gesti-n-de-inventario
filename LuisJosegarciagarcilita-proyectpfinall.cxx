#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib> // Para usar system()
using namespace std;

// estructura para representar un producto
struct Producto
{
	int id;
	string nombre;
	int cantidad;
	float precio;
	Producto *siguiente;
};

// clase para manejar la lista enlazada de productos
class Inventario
{
  private:
	Producto *inicio;

  public:
	Inventario() : inicio(nullptr) {}

	//funcion para agregar un producto al inventario
	void agregarProducto(int id, string nombre, int cantidad, float precio)
	{
		Producto *nuevo = new Producto{id, nombre, cantidad, precio, nullptr};

		if (!inicio)
		{
			inicio = nuevo;
		}
		else
		{
			Producto *temp = inicio;
			while (temp->siguiente)
			{
				temp = temp->siguiente;
			}
			temp->siguiente = nuevo;
		}
		cout << "Producto agregado exitosamente.\n";
	}

	// función para eliminar un producto por ID
	void eliminarProducto(int id)
	{
		if (!inicio)
		{
			cout << "El inventario está vacío.\n";
			return;
		}

		if (inicio->id == id)
		{
			Producto *temp = inicio;
			inicio = inicio->siguiente;
			delete temp;
			cout << "Producto eliminado exitosamente.\n";
			return;
		}

		Producto *actual = inicio;
		Producto *anterior = nullptr;
		while (actual && actual->id != id)
		{
			anterior = actual;
			actual = actual->siguiente;
		}

		if (!actual)
		{
			cout << "Producto no encontrado.\n";
		}
		else
		{
			anterior->siguiente = actual->siguiente;
			delete actual;
			cout << "Producto eliminado exitosamente.\n";
		}
	}

	// función para modificar un producto por ID
	void modificarProducto(int id, int nuevaCantidad, float nuevoPrecio)
	{
		Producto *temp = inicio;
		while (temp)
		{
			if (temp->id == id)
			{
				temp->cantidad = nuevaCantidad;
				temp->precio = nuevoPrecio;
				cout << "Producto modificado exitosamente.\n";
				return;
			}
			temp = temp->siguiente;
		}
		cout << "Producto no encontrado.\n";
	}

	// función para buscar un producto por ID
	void buscarProducto(int id)
	{
		Producto *temp = inicio;
		while (temp)
		{
			if (temp->id == id)
			{
				cout << "ID: " << temp->id << "\nNombre: " << temp->nombre
					 << "\nCantidad: " << temp->cantidad
					 << "\nPrecio: $" << fixed << setprecision(2) << temp->precio << "\n";
				return;
			}
			temp = temp->siguiente;
		}
		cout << "Producto no encontrado.\n";
	}

	//función para mostrar los productos ordenados por cantidad
	void mostrarProductosOrdenados()
	{
		if (!inicio)
		{
			cout << "El inventario está vacío.\n";
			return;
		}

		Producto *copia = nullptr;
		Producto *temp = inicio;
		while (temp)
		{
			Producto *nuevo = new Producto{temp->id, temp->nombre, temp->cantidad, temp->precio, nullptr};
			if (!copia || nuevo->cantidad < copia->cantidad)
			{
				nuevo->siguiente = copia;
				copia = nuevo;
			}
			else
			{
				Producto *aux = copia;
				while (aux->siguiente && aux->siguiente->cantidad <= nuevo->cantidad)
				{
					aux = aux->siguiente;
				}
				nuevo->siguiente = aux->siguiente;
				aux->siguiente = nuevo;
			}
			temp = temp->siguiente;
		}

		temp = copia;
		cout << "Productos ordenados por cantidad en stock:\n";
		while (temp)
		{
			cout << "ID: " << temp->id << ", Nombre: " << temp->nombre
				 << ", Cantidad: " << temp->cantidad
				 << ", Precio: $" << fixed << setprecision(2) << temp->precio << "\n";
			temp = temp->siguiente;
		}

		while (copia)
		{
			Producto *aux = copia;
			copia = copia->siguiente;
			delete aux;
		}
	}

	//función para mostrar toda la lista de productos
	void mostrarProductos()
	{
		if (!inicio)
		{
			cout << "El inventario está vacío.\n";
			return;
		}

		Producto *temp = inicio;
		cout << "Lista de productos en el inventario:\n";
		while (temp)
		{
			cout << "ID: " << temp->id << ", Nombre: " << temp->nombre
				 << ", Cantidad: " << temp->cantidad
				 << ", Precio: $" << fixed << setprecision(2) << temp->precio << "\n";
			temp = temp->siguiente;
		}
	}

	// función para mostrarvalertas por bajo stock
	void alertasBajoStock(int limite)
	{
		Producto *temp = inicio;
		bool hayAlertas = false;

		cout << "Productos con bajo stock (menos de " << limite << " unidades):\n";
		while (temp)
		{
			if (temp->cantidad < limite)
			{
				cout << "ID: " << temp->id << ", Nombre: " << temp->nombre
					 << ", Cantidad: " << temp->cantidad << "\n";
				hayAlertas = true;
			}
			temp = temp->siguiente;
		}

		if (!hayAlertas)
		{
			cout << "No hay productos con bajo stock.\n";
		}
	}

	// función para exportar inventario a un archivo CSV
	void exportarCSV()
	{
		ofstream archivo("inventario.csv");
		if (!archivo)
		{
			cout << "Error al crear el archivo.\n";
			return;
		}

		archivo << "ID,Nombre,Cantidad,Precio\n";
		Producto *temp = inicio;
		while (temp)
		{
			archivo << temp->id << "," << temp->nombre << "," << temp->cantidad
					<< "," << fixed << setprecision(2) << temp->precio << "\n";
			temp = temp->siguiente;
		}

		archivo.close();
		cout << "Datos exportados exitosamente a 'inventario.csv'.\n";
	}

	//destructor para liberar memoria
	~Inventario()
	{
		while (inicio)
		{
			Producto *temp = inicio;
			inicio = inicio->siguiente;
			delete temp;
		}
	}
};

// función para limpiar pantalla
void limpiarPantalla()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

//función principal para interactuar con el sistema de inventarios
int main()
{
	Inventario inventario;
	int opcion, id, cantidad, limite;
	string nombre;
	float precio;

	do
	{
		limpiarPantalla();

		cout << "--- Sistema de Gestión de Inventarios ---\n";
		cout << "1. Agregar producto\n";
		cout << "2. Eliminar producto\n";
		cout << "3. Modificar producto\n";
		cout << "4. Buscar producto por ID\n";
		cout << "5. Mostrar productos ordenados por cantidad\n";
		cout << "6. Mostrar toda la lista de productos\n";
		cout << "7. Mostrar alertas de bajo stock\n";
		cout << "8. Exportar inventario a CSV\n";
		cout << "9. Salir\n";
		cout << "Seleccione una opción: ";
		cin >> opcion;

		switch (opcion)
		{
		case 1:
			cout << "Ingrese ID: ";
			cin >> id;
			cin.ignore();
			cout << "Ingrese nombre: ";
			getline(cin, nombre);
			cout << "Ingrese cantidad: ";
			cin >> cantidad;
			cout << "Ingrese precio: ";
			cin >> precio;
			inventario.agregarProducto(id, nombre, cantidad, precio);
			break;

		case 2:
			cout << "Ingrese ID del producto a eliminar: ";
			cin >> id;
			inventario.eliminarProducto(id);
			break;

		case 3:
			cout << "Ingrese ID del producto a modificar: ";
			cin >> id;
			cout << "Ingrese nueva cantidad: ";
			cin >> cantidad;
			cout << "Ingrese nuevo precio: ";
			cin >> precio;
			inventario.modificarProducto(id, cantidad, precio);
			break;

		case 4:
			cout << "Ingrese ID del producto a buscar: ";
			cin >> id;
			inventario.buscarProducto(id);
			break;

		case 5:
			inventario.mostrarProductosOrdenados();
			break;

		case 6:
			inventario.mostrarProductos();
			break;

		case 7:
			cout << "Ingrese el límite de bajo stock: ";
			cin >> limite;
			inventario.alertasBajoStock(limite);
			break;

		case 8:
			inventario.exportarCSV();
			break;

		case 9:
			cout << "Saliendo del sistema...\n";
			break;

		default:
			cout << "Opción no válida.\n";
		}

		if (opcion != 9)
		{
			cout << "\nPresione Enter para continuar...";
			cin.ignore();
			cin.get();
		}
	} while (opcion != 9);

	return 0;
}