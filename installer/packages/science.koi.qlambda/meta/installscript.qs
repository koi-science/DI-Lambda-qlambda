
function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{

    component.createOperations();


    component.addOperation("CreateShortcut", "@TargetDir@/QLambda.exe", "@StartMenuDir@/QLambda.lnk",
        "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/qlambda_96x96.ico", "description=Qlambda App");

}
