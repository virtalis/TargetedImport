local Assembly_Import_Target = vrTreeRoot():find("root/Scenes/Import%20Target")
local Library_Import_Target = vrTreeRoot():find("root/Libraries/Import%20Target")

local filepath = "C:\\d.corlett\\visren\\resources\\models\\inventor\\Fishing Rod\\Workspaces\\Crankshaft Assembly.iam"

print(filepath)
print(Assembly_Import_Target)
print(Library_Import_Target)
print(TargetedImport.Import(filepath, Assembly_Import_Target, Library_Import_Target, 'Autodesk Inventor Importer'))