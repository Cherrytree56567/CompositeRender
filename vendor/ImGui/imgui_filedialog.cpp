/*
	Copyright 2020 Limeoats
	Original project: https://github.com/Limeoats/L2DFileDialog

	Changes by Vladimir Sigalkin
*/

#pragma once

#include <string>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <cstring>

#include <imgui.h>
#include "imgui_filedialog.h"

using namespace std::chrono_literals;

typedef int ImGuiFileDialogSortOrder;

enum ImGuiFileDialogSortOrder_
{
	ImGuiFileDialogSortOrder_Up,
	ImGuiFileDialogSortOrder_Down,
	ImGuiFileDialogSortOrder_None
};

void ImFileDialogInfo::refreshPaths()
{
	refreshInfo = false;
	currentIndex = 0;
	currentFiles.clear();
	currentDirectories.clear();

	try {
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directoryPath))
		{
			if (entry.is_directory())
			{
				currentDirectories.push_back(entry);
			}
			else
			{
				if (fileFilterFunc != nullptr) {
					if (!fileFilterFunc(entry.path().filename().string())) {
						continue;
					}
				}
				currentFiles.push_back(entry);
			}
		}
	} catch (const std::exception& e) {
		// Handle invalid directory paths or permissions
		currentDirectories.clear();
		currentFiles.clear();
	}
}

bool ImGui::FileDialog(bool* open, ImFileDialogInfo* dialogInfo)
{
	if (!*open) return false;

	assert(dialogInfo != nullptr);

	bool complete = false;

	ImGui::PushID(dialogInfo);
	ImGui::SetNextWindowSize(ImVec2(740.0f, 430.0f), ImGuiCond_FirstUseEver);

	if (ImGui::Begin(dialogInfo->title.c_str(), open))
	{
		if (dialogInfo->currentFiles.empty() && dialogInfo->currentDirectories.empty() || dialogInfo->refreshInfo)
			dialogInfo->refreshPaths();

		// Editable Path Input
		static char pathBuffer[256];
		std::string currentPath = dialogInfo->directoryPath.string();
		std::strncpy(pathBuffer, currentPath.c_str(), sizeof(pathBuffer) - 1);
		pathBuffer[sizeof(pathBuffer) - 1] = '\0';

		if (ImGui::InputText("Path", pathBuffer, sizeof(pathBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::filesystem::path newPath(pathBuffer);
			if (std::filesystem::exists(newPath) && std::filesystem::is_directory(newPath)) {
				dialogInfo->directoryPath = newPath;
				dialogInfo->refreshInfo = true;
			}
		}

		ImGui::BeginChild("##browser", ImVec2(ImGui::GetContentRegionAvail().x, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::Columns(3);

		// File Columns
		if (ImGui::Selectable("Name")) {}
		ImGui::NextColumn();
		if (ImGui::Selectable("Size")) {}
		ImGui::NextColumn();
		if (ImGui::Selectable("Type")) {}
		ImGui::NextColumn();
		ImGui::Separator();

		// Draw Directories
		for (size_t i = 0; i < dialogInfo->currentDirectories.size(); ++i)
		{
			auto directoryEntry = dialogInfo->currentDirectories[i];
			auto directoryName = directoryEntry.path().filename();

			if (ImGui::Selectable(directoryName.string().c_str(), dialogInfo->currentIndex == i, ImGuiSelectableFlags_AllowDoubleClick))
			{
				dialogInfo->currentIndex = i;
				if (ImGui::IsMouseDoubleClicked(0))
				{
					dialogInfo->directoryPath = directoryEntry.path();
					dialogInfo->refreshInfo = true;
				}
			}
			ImGui::NextColumn();
			ImGui::TextUnformatted("-");
			ImGui::NextColumn();
			ImGui::TextUnformatted("<directory>");
			ImGui::NextColumn();
		}

		// Draw Files
		for (size_t i = 0; i < dialogInfo->currentFiles.size(); ++i)
		{
			auto fileEntry = dialogInfo->currentFiles[i];
			auto fileName = fileEntry.path().filename();

			if (ImGui::Selectable(fileName.string().c_str(), dialogInfo->currentIndex == i, ImGuiSelectableFlags_AllowDoubleClick))
			{
				dialogInfo->currentIndex = i;
				dialogInfo->fileName = fileName;
			}
			ImGui::NextColumn();
			ImGui::TextUnformatted(std::to_string(fileEntry.file_size()).c_str());
			ImGui::NextColumn();
			ImGui::TextUnformatted(fileEntry.path().extension().string().c_str());
			ImGui::NextColumn();
		}
		ImGui::EndChild();

		// File Name Input
		static char fileNameBuffer[200];
		std::strncpy(fileNameBuffer, dialogInfo->fileName.string().c_str(), sizeof(fileNameBuffer) - 1);
		fileNameBuffer[sizeof(fileNameBuffer) - 1] = '\0';
		ImGui::InputText("File Name", fileNameBuffer, sizeof(fileNameBuffer));
		dialogInfo->fileName = std::string(fileNameBuffer);

		if (ImGui::Button("Cancel")) *open = false;
		ImGui::SameLine();
		if (ImGui::Button("Open")) {
			dialogInfo->resultPath = dialogInfo->directoryPath / dialogInfo->fileName;
			complete = true;
			*open = false;
		}
	}
	ImGui::End();
	ImGui::PopID();
	return complete;
}
