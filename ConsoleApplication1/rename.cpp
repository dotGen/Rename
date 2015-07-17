#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <list>

void rename_files(std::list<std::string>& list, std::string rename_pattern) {

	char old_name [_MAX_FNAME];
	char directory [_MAX_DIR];
	char drive[_MAX_DRIVE];
	char ext[_MAX_EXT];

	for (std::list<std::string>::iterator file = list.begin(); file != list.end(); ++file) {
		_splitpath_s((*file).c_str(), drive, directory, old_name, ext);
		
		std::string new_name;

		new_name.append(drive);
		new_name.append(directory);
		new_name.append(rename_pattern);
		new_name.append(old_name);
		new_name.append(ext);

		rename((*file).c_str(), new_name.c_str());
	}
}

void get_files_from_directory(std::list<std::string> & list, std::string directory, std::string ext, std::string rename_pattern, boolean matching_mode = true) {

	if (matching_mode) get_files_from_directory(list, directory, ext, rename_pattern,  false);

	std::string directory2 = directory + "\\" + ((!matching_mode) ? "*" : "*."+ext);

	WIN32_FIND_DATAA data;
	HANDLE hFind = FindFirstFileA(directory2.c_str(), &data);

	while (hFind != INVALID_HANDLE_VALUE)
	{
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0 && data.cFileName[0] != '.')
			get_files_from_directory(list, directory + "\\" + data.cFileName, ext, rename_pattern);
		else
			if (matching_mode) {
				list.push_front(directory + "\\" + data.cFileName);
				std::cout << data.cFileName << std::endl;
			}
		if (FindNextFileA(hFind, &data) == FALSE)
			break;
	}
	FindClose(hFind);
}

int _tmain(int argc, TCHAR* argv[])
{

	using namespace std;

	HANDLE hFind;
	WIN32_FIND_DATAA data;

	cout << "Introduce el directorio desde donde realizar la busqueda : [Ejemplo: c:\\users] " << endl;

	string dir, ext, ren, res;
	getline(cin, dir);

	cout << "Introduce la extension de archivo que quiere sobreescribir : [Ejemplo: jpg o pdf] " << endl;

	getline(cin, ext);

	cout << "Ristra a concatenar al principio : [Ejemplo: TS o patron]" << endl;

	getline(cin, ren);

	cout << "Archivos que se modificaran ..." << endl;

	std::list<std::string> list;

	get_files_from_directory(list, dir.c_str(), ext.c_str(), ren.c_str());

	if (!list.empty()) {
		cout << "Confirma los cambios que se van a realizar? Y/N" << endl;

		getline(cin, res);

		if (res.compare("y") == 0 || res.compare("Y") == 0) {
			rename_files(list, ren.c_str());
			cout << "Cambios realizados con exito " << endl;
		} else
			cout << "No se ha realizado ningun cambio" << endl;
	} else {
		cout << "No se han encontrado archivos a modificar" << endl;
	}

	system("PAUSE");
	return 0;
}



