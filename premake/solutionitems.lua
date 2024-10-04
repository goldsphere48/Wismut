require("vstudio")

premake.api.register {
	name = "solutionitems",
	scope = "workspace",
	kind = "list:string"
}

premake.override(premake.vstudio.sln2005, "projects", function(base, wks)
	if wks.solutionitems and #wks.solutionitems > 0 then
		-- https://stackoverflow.com/questions/2327202/what-are-the-project-guids-in-a-visual-studio-solution-file-used-for
		local solution_folder_guid = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}"
		premake.push("Project(\"" .. solution_folder_guid .. "\") = \"Solution Items\", \"Solution Items\", \"{" .. os.uuid("Solution Items:" .. wks.name) .. "}\"")
		premake.push("ProjectSection(SolutionItems) = preProject")
		for _, path in ipairs(wks.solutionitems) do
			print(path)
			premake.w(path .. "=" .. path)
		end
		premake.pop("EndProjectSection")
		premake.pop("EndProject")
	end
	base(wks)
end)
