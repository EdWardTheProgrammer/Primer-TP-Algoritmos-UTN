#include <iostream>

using namespace std;

struct TCentroMaestro
{
    int idCentro;
    char nombre[50];
    float stockPlastico;
    float stockVidrio;
    float stockCarton;
};

struct TEntregaDiaria
{
    int idCentro;
    char tipoMaterial;
    float peso;
};

void inicializarMaestro(TCentroMaestro maestro[]){
	maestro[0] = {101, "Centro A", 0.0, 0.0, 0.0};
	maestro[1] = {102, "Centro B", 0.0, 0.0, 0.0};
	maestro[2] = {103, "Centro C", 0.0, 0.0, 0.0};
	maestro[3] = {104, "Centro D", 0.0, 0.0, 0.0};
	maestro[4] = {105, "Centro E", 0.0, 0.0, 0.0};
	maestro[5] = {106, "Centro F", 0.0, 0.0, 0.0};
	maestro[6] = {107, "Centro G", 0.0, 0.0, 0.0};
	maestro[7] = {110, "Centro H", 0.0, 0.0, 0.0};	
}

int elegirOpcion();
void inicializarMaestro(TCentroMaestro maestro[]);
void cargarYValidarEntregas(TCentroMaestro maestro[], int cantCentros, TEntregaDiaria novedad[], int &cantNovedades);
bool existeId(TCentroMaestro maestro[], int cantCentros, int idIngresado);
int ingresarEntero(int min, int max = 0, bool controlarMax = false);
void actualizarStockSemanal(TCentroMaestro maestro[], int cantCentros, TEntregaDiaria novedad[], int cantNovedades);
void MostrarEstadisticas(TCentroMaestro maestro[], int cantCentros);
void totalGeneralDeKg(TCentroMaestro maestro[], int cantCentros, float &contKgTotal);
void centroConMayorVolumen(TCentroMaestro maestro[], int cantCentros);
void porcentajeDeCadaTipo(TCentroMaestro maestro[], int cantCentros, float contKgTotal);


int main()
{
    TCentroMaestro maestroA[100];
    int cantCentros = 8;

    TEntregaDiaria novedadesB[50];
    int cantNovedades = 0;
    
    inicializarMaestro(maestroA);

    int decision;
    cout << "Bienvenido al sistema de gestion logistica de los Puntos Verdes" << endl;

    do
    {
        decision = elegirOpcion();

        switch (decision)
        {
        case 1:
            cargarYValidarEntregas(maestroA, cantCentros, novedadesB, cantNovedades);
            break;

        case 2:
            actualizarStockSemanal(maestroA, cantCentros, novedadesB, cantNovedades);
            break;

        case 3:
            MostrarEstadisticas (maestroA, cantCentros);
            break;

        case 4:
            cout << "Saliendo del sistema de gestion." << endl;
            break;
        }
    } while (decision != 4);

    return 0;
}

int elegirOpcion()
{
    cout << "1) Cargar y Validar Entregas" << endl;
    cout << "2) Actualizar Stock Semanal" << endl;
    cout << "3) Mostrar Estadisticas" << endl;
    cout << "4) Salir del sistema" << endl;
    cout << "Seleccione una opcion: ";
    int opcion = ingresarEntero(1, 4, true);
    return opcion;
}

void cargarYValidarEntregas(TCentroMaestro maestro[], int cantCentros, TEntregaDiaria novedad[], int &cantNovedades)
{
    int id;
    cantNovedades = 0;
    do
    {
        cout << "Ingrese ID de Centro (0 para finalizar la carga): ";
        cin >> id;

        if (id != 0)
        {
            if (existeId(maestro, cantCentros, id))
            {

                novedad[cantNovedades].idCentro = id;

                char mat;
                do
                {
                    cout << "Ingrese Tipo de Material ('P'=Plastico, 'V'=Vidrio, 'C'=Carton): ";
                    cin >> mat;
                    mat = toupper(mat);
                    if (mat != 'P' && mat != 'V' && mat != 'C')
                    {
                        cout << "Material invalido. Pruebe otra vez." << endl;
                    }
                } while (mat != 'P' && mat != 'V' && mat != 'C');
                novedad[cantNovedades].tipoMaterial = mat;

                cout << "Ingrese Peso (kg): ";
                cin >> novedad[cantNovedades].peso;

                cantNovedades++;
            }
            else
            {
                cout << "Error: El ID de Centro " << id << " no existe en la base maestra." << endl;
            }
        }
    } while (id != 0 && cantNovedades < 50);

    if (cantNovedades > 0)
    {

        int i = 0;
        while (i < cantNovedades)
        {
            int idActual = novedad[i].idCentro;
            float totalKgCentro = 0.0;

            cout << "Centro ID: " << idActual << endl;

            while (i < cantNovedades && novedad[i].idCentro == idActual)
            {
                cout << "Material: " << novedad[i].tipoMaterial
                     << " | Peso: " << novedad[i].peso << " kg" << endl;

                totalKgCentro += novedad[i].peso;
                i++;
            }

            cout << "Total acumulado por Centro " << idActual << ": " << totalKgCentro << " kg" << endl;
        }
    }
    else
    {
        cout << "No se ingresaron novedades" << endl;
    }
}

bool existeId(TCentroMaestro maestro[], int cantCentros, int idIngresado)
{
    int fin = cantCentros - 1;

    for (int inicio = 0; inicio <= fin;)
    {
        int medio = inicio + (fin - inicio) / 2;

        if (maestro[medio].idCentro == idIngresado)
            return true;

        if (maestro[medio].idCentro < idIngresado)
            inicio = medio + 1;
        else
            fin = medio - 1;
    }
    return false;
}

int ingresarEntero(int min, int max, bool controlarMax)
{
    int valor;
    bool esValido;
    do
    {
        cin >> valor;

        esValido = (valor >= min) && (!controlarMax || valor <= max);

        if (!esValido)
        {
            cout << "Error. Ingrese un valor valido: ";
        }
    } while (!esValido);

    return valor;
}

void actualizarStockSemanal(TCentroMaestro maestro[], int cantCentros, TEntregaDiaria novedad[], int cantNovedades)
{
    int i = 0;
    int j = 0;
    // se recorre ambas estructuras en paralelo, ya que ambas estan ordenadas por idCentro
    while (i < cantCentros && j < cantNovedades)
    {
        if (maestro[i].idCentro == novedad[j].idCentro)
        {
            if (novedad[j].tipoMaterial == 'V')
            {
                maestro[i].stockVidrio = maestro[i].stockVidrio + novedad[j].peso;
            }
            else if (novedad[j].tipoMaterial == 'P')
            {
                maestro[i].stockPlastico = maestro[i].stockPlastico + novedad[j].peso;
            }
            else if (novedad[j].tipoMaterial == 'C')
            {
                maestro[i].stockCarton = maestro[i].stockCarton + novedad[j].peso;
            }
            j++; // se avanza solo en novedades, por si el mismo centro tiene más entregas pendientes de procesar
        }
        else if (maestro[i].idCentro < novedad[j].idCentro)
        {
            i++;
        }
        else if (maestro[i].idCentro > novedad[j].idCentro)
        {
            j++;
        }
    }
}

void MostrarEstadisticas(TCentroMaestro maestro[], int cantCentros)
{
    cout << "Estadisticas del Sistema:" << endl;
    
    float contKgTotal = 0.0;
    totalGeneralDeKg(maestro, cantCentros, contKgTotal);
    cout << "Total general de kilogramos en el sistema: " << contKgTotal << "kg." << endl;
    
    centroConMayorVolumen(maestro, cantCentros);
    porcentajeDeCadaTipo(maestro, cantCentros, contKgTotal);
}

void totalGeneralDeKg(TCentroMaestro maestro[], int cantCentros, float &contKgTotal)
{
    contKgTotal = 0;

    for(int i = 0; i < cantCentros; i++)
    {
        contKgTotal += maestro[i].stockPlastico;
        contKgTotal += maestro[i].stockVidrio;
        contKgTotal += maestro[i].stockCarton;
    }
}

void centroConMayorVolumen(TCentroMaestro maestro[], int cantCentros){
	float contKgAcumulados = 0;
	float mayor = 0;
	int indiceMayor = 0;
		for (int i = 0; i < cantCentros; i++){
			contKgAcumulados = maestro[i].stockPlastico + maestro[i].stockCarton + maestro[i].stockVidrio;
			if (contKgAcumulados > mayor){
				mayor = contKgAcumulados;
				indiceMayor = i;	
			}
		}
	cout << "Nombre del Centro con mayor volumen de carga acumulada: " << maestro[indiceMayor].nombre << ". Acumula " << mayor << "kg." << endl;
}

void porcentajeDeCadaTipo(TCentroMaestro maestro[], int cantCentros, float contKgTotal)
{
    float totalP = 0.0, totalV = 0.0, totalC = 0.0;

    for(int i = 0; i < cantCentros; i++)
    {
        totalP += maestro[i].stockPlastico;
        totalV += maestro[i].stockVidrio;
        totalC += maestro[i].stockCarton;
    }
    cout << "Porcentaje de cada tipo de material:" << endl;
    cout << "Plastico: " << (totalP / contKgTotal) * 100 << "%" << endl;
    cout << "Vidrio: " << (totalV / contKgTotal) * 100 << "%" << endl;
    cout << "Carton: " << (totalC / contKgTotal) * 100 << "%" << endl;
}
