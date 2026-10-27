# 按当前窗口的代码页选择字符集，编译并运行。
# 编译期的字符集只影响 exe 里的中文字面量；窗口代码页默认不动，只有下面两种
# 无法被任何单一字符集满足的情况例外（临时统一切到 936，结束还原）。

Set-Location -LiteralPath $PSScriptRoot

$inCp = [Console]::InputEncoding.CodePage
$outCp = [Console]::OutputEncoding.CodePage
$modernTerminal = [bool]$env:WT_SESSION -or [bool]$env:TERM_PROGRAM
$restoreIn = 0
$restoreOut = 0

# 情形一：输出与输入代码页不一致（chcp 只显示输入代码页，两者可能不同）。
#         此时 UTF-8 编出来的名字会被当成 GBK 收进来，GBK 编出来的字面量又被
#         当成 UTF-8 显示，两条路都不通。
# 情形二：老式控制台窗口（conhost）的输出/输入都是 65001。这种窗口收不到正确
#         的中文输入（汉字会变成 0xC0 之类的字节），同样没有可用组合。
if (($outCp -ne $inCp) -or ($outCp -eq 65001 -and -not $modernTerminal))
{
    if ($outCp -ne $inCp)
    {
        Write-Host ("检测到窗口的代码页不一致：输出 {0}、输入 {1}（chcp 显示的只是输入）。" -f $outCp, $inCp) -ForegroundColor Yellow
    }
    else
    {
        Write-Host "检测到老式控制台窗口（conhost）的 65001 模式：这种窗口收不到正确的中文输入。" -ForegroundColor Yellow
    }

    Write-Host "无论按哪种字符集编译都会有半边乱码，现将该窗口临时统一为 936，结束后还原（不会清屏）。" -ForegroundColor Yellow

    try
    {
        [Console]::OutputEncoding = [Text.Encoding]::GetEncoding(936)
        [Console]::InputEncoding = [Text.Encoding]::GetEncoding(936)
        $restoreOut = $outCp
        $restoreIn = $inCp
        $outCp = 936
        $inCp = 936
    }
    catch
    {
        Write-Host "临时切换失败，仍按当前状态编译；中文可能异常。" -ForegroundColor Yellow
    }
}

$useGbk = ($outCp -ne 65001)
$sources = @('main.c', 'library.c', 'book_io.c', 'DoubleLinkedList.c')

try
{
    $missing = @($sources | Where-Object { -not (Test-Path -LiteralPath (Join-Path $PSScriptRoot $_)) })
    if ($missing.Count -gt 0)
    {
        Write-Host ("缺少源文件：{0}" -f ($missing -join '、')) -ForegroundColor Red
        exit 1
    }

    if (-not (Get-Command gcc -ErrorAction SilentlyContinue))
    {
        Write-Host "未找到 gcc，请确认 MSYS2 的 C:\msys64\ucrt64\bin 已加入 PATH。" -ForegroundColor Red
        exit 1
    }

    $gccArgs = @('-std=c11', '-Wall', '-Wextra', '-pedantic', '-O2')
    if ($useGbk)
    {
        $gccArgs += '-fexec-charset=GBK'
    }
    $gccArgs += $sources
    $gccArgs += @('-o', 'books.exe')

    $mode = if ($useGbk) { 'GBK' } else { 'UTF-8' }
    Write-Host ("正在编译（输出代码页 {0}、输入代码页 {1}，按 {2} 编译）：" -f $outCp, $inCp, $mode) -ForegroundColor Cyan
    Write-Host ("gcc " + ($gccArgs -join ' '))

    & gcc @gccArgs
    if ($LASTEXITCODE -ne 0)
    {
        Write-Host "编译失败，请查看上面的报错信息。" -ForegroundColor Red
        exit 1
    }

    Write-Host "编译成功，已生成 books.exe。" -ForegroundColor Green

    if ($args.Count -gt 0 -and $args[0] -eq 'build')
    {
        Write-Host "本次只编译不运行；要运行请执行 .\books.exe" -ForegroundColor Cyan
        exit 0
    }

    Write-Host "正在本窗口运行 books.exe ..." -ForegroundColor Cyan
    & (Join-Path $PSScriptRoot 'books.exe')
    exit $LASTEXITCODE
}
finally
{
    if ($restoreOut -ne 0 -or $restoreIn -ne 0)
    {
        try
        {
            if ($restoreOut -ne 0) { [Console]::OutputEncoding = [Text.Encoding]::GetEncoding($restoreOut) }
            if ($restoreIn -ne 0) { [Console]::InputEncoding = [Text.Encoding]::GetEncoding($restoreIn) }
        }
        catch
        {
        }
    }
}