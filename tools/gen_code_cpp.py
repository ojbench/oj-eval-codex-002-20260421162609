#!/usr/bin/env python3
import pathlib

root = pathlib.Path(__file__).resolve().parents[1]
hdr = root / 'src' / 'include' / 'int2048.h'
impl = root / 'src' / 'int2048.cpp'
out = root / 'code.cpp'

header_text = hdr.read_text()
impl_text = impl.read_text()

# Remove include of header in impl, and include standard headers
impl_text = impl_text.replace('#include "include/int2048.h"', '')

preamble = '\n#include <bits/stdc++.h>\nusing namespace std;\n\n'

out.write_text(preamble + header_text + '\n' + impl_text)
print(f'Wrote {out}')

