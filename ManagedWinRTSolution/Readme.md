# ManagedWinRTSolution

**Important note**
If the solution does not compile because of "C:\Microsoft.Cpp.Default.props" is not found, the problem is because the environment variable `VCTargetsPath` is not resolved correctly.
In this case:
- open the Windows environment variable editor
- set the variable `VCTargetsPath` to `C:\Program Files (x86)\Microsoft Visual Studio\2019\Preview\MSBuild\Microsoft\VC\v160`
> This is the path on my PC for VS2019 Preview. If you have any other version, the path is different!
> Check the path on your installation

