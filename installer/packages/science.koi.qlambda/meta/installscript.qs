
function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{

    component.createOperations();


    component.addOperation("CreateShortcut", "@TargetDir@/QLambda.exe", "@StartMenuDir@/QLambda.lnk",
        "workingDirectory=@TargetDir@");
}
