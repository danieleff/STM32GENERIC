echo Waiting for %1 serial...

for /l %%x in (1, 1, 40) do (
  ping -w 100 -n 1 192.0.2.1 > nul
  mode %1 > nul
  if ERRORLEVEL 0 goto comPortFound
)

echo timeout waiting for %1 serial

:comPortFound
