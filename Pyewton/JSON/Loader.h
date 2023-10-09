#ifndef JSON_LOADER_H
#define JSON_LOADER_H

#include "../Body/Body.h"

#include <folly/dynamic.h>
#include <folly/json.h>
#include <iostream>
#include <fstream>
#include "../A3rdParty/imgui-notify/imgui_notify.h"

namespace Pyewton::Json
{
	Pyewton::BodyList LoadFile(char* path);
	void SaveToFile(BodyList& bodylist, std::filesystem::path path);
}



#endif // !JSON_LOADER_H
