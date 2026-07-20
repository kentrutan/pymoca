#!/usr/bin/env bash
#
# Parameterized RTC-Tools x pymoca benchmark driver.
#
# Builds two throwaway virtualenvs with an identical RTC-Tools + solver stack,
# differing only in the pymoca version, runs the whole RTC-Tools example suite
# through both (cold-compiling every model), and writes a markdown report
# comparing pymoca compile time, flattened-model structure, and numerical
# results.
#
# Each pymoca "ref" is either:
#   * a pip spec              e.g.  0.9.2   or  pymoca==0.9.2
#   * a path to a checkout    e.g.  /path/to/pymoca   (installed editable, --no-deps)
#
# Usage:
#   run_benchmark.sh \
#       --pymoca-a 0.9.2         --label-a 0.9.2 \
#       --pymoca-b /path/to/pr   --label-b pr \
#       [--rtctools rtc-tools]   [--rtc-src /path/to/rtc-tools/checkout] \
#       [--workdir ./bench-work] [--only basic,mixed_integer] [--timeout 900]
#
# Defaults reproduce the original comparison: pymoca 0.9.2 (RTC-Tools' pin)
# versus the fix-inherited-symbol-scope-pr branch.

set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PYMOCA_A="0.9.2"; LABEL_A="0.9.2"
PYMOCA_B=""; LABEL_B="pr"
RTCTOOLS="rtc-tools"
RTC_SRC=""
WORKDIR="./bench-work"
ONLY=""
TIMEOUT="900"

while [[ $# -gt 0 ]]; do
  case "$1" in
    --pymoca-a) PYMOCA_A="$2"; shift 2;;
    --pymoca-b) PYMOCA_B="$2"; shift 2;;
    --label-a) LABEL_A="$2"; shift 2;;
    --label-b) LABEL_B="$2"; shift 2;;
    --rtctools) RTCTOOLS="$2"; shift 2;;
    --rtc-src) RTC_SRC="$2"; shift 2;;
    --workdir) WORKDIR="$2"; shift 2;;
    --only) ONLY="$2"; shift 2;;
    --timeout) TIMEOUT="$2"; shift 2;;
    *) echo "unknown arg: $1" >&2; exit 2;;
  esac
done

if [[ -z "$PYMOCA_B" ]]; then
  echo "error: --pymoca-b is required (path to the pymoca branch under test)" >&2
  exit 2
fi

mkdir -p "$WORKDIR"
WORKDIR="$(cd "$WORKDIR" && pwd)"

# --- RTC-Tools examples source ---
if [[ -z "$RTC_SRC" ]]; then
  RTC_SRC="$WORKDIR/rtc-tools"
  if [[ ! -d "$RTC_SRC" ]]; then
    echo "cloning rtc-tools ..."
    git clone --depth 1 https://github.com/rtc-tools/rtc-tools.git "$RTC_SRC"
  fi
fi
EXAMPLES="$RTC_SRC/examples"

install_pymoca() {  # <venv_python_dir> <ref>
  local pip="$1/pip"; local ref="$2"
  if [[ -d "$ref" ]]; then
    "$pip" install -q --no-deps -e "$ref"
  else
    "$pip" install -q --force-reinstall --no-deps "$ref"
  fi
}

build_venv() {  # <venv_dir> <pymoca_ref>
  local venv="$1"; local ref="$2"
  python3 -m venv "$venv"
  "$venv/bin/pip" install -q --upgrade pip
  "$venv/bin/pip" install -q "$RTCTOOLS"
  install_pymoca "$venv/bin" "$ref"
}

echo "=== building venv A ($LABEL_A) ==="
build_venv "$WORKDIR/venv_a" "$PYMOCA_A"
echo "=== building venv B ($LABEL_B) ==="
build_venv "$WORKDIR/venv_b" "$PYMOCA_B"

versions() {  # <venv_python>
  "$1" -c 'import pymoca,casadi,rtctools,numpy,scipy;print("pymoca",pymoca.__version__);print("casadi",casadi.__version__);print("rtctools",rtctools.__version__);print("numpy",numpy.__version__);print("scipy",scipy.__version__)'
}

RESULTS="$WORKDIR/results"
mkdir -p "$RESULTS"

ONLY_ARG=()
[[ -n "$ONLY" ]] && ONLY_ARG=(--only "$ONLY")

echo "=== running suite under $LABEL_A ==="
"$WORKDIR/venv_a/bin/python" "$HERE/run_all.py" \
  --python "$WORKDIR/venv_a/bin/python" --examples "$EXAMPLES" \
  --label "$LABEL_A" --out "$RESULTS/$LABEL_A" --timeout "$TIMEOUT" "${ONLY_ARG[@]}"

echo "=== running suite under $LABEL_B ==="
"$WORKDIR/venv_b/bin/python" "$HERE/run_all.py" \
  --python "$WORKDIR/venv_b/bin/python" --examples "$EXAMPLES" \
  --label "$LABEL_B" --out "$RESULTS/$LABEL_B" --timeout "$TIMEOUT" "${ONLY_ARG[@]}"

# --- metadata header ---
META="$RESULTS/meta.json"
{
  echo "{"
  echo "  \"generated\": \"$(date -u +%Y-%m-%dT%H:%M:%SZ)\","
  echo "  \"rtc_tools_examples\": \"$EXAMPLES\","
  echo "  \"$LABEL_A\": \"$(versions "$WORKDIR/venv_a/bin/python" | tr '\n' ';')\","
  echo "  \"$LABEL_B\": \"$(versions "$WORKDIR/venv_b/bin/python" | tr '\n' ';')\""
  echo "}"
} > "$META"

echo "=== writing RESULTS.md ==="
"$WORKDIR/venv_a/bin/python" "$HERE/compare.py" \
  --base-dir "$RESULTS/$LABEL_A" --pr-dir "$RESULTS/$LABEL_B" \
  --base-label "$LABEL_A" --pr-label "$LABEL_B" \
  --meta "$META" --out "$WORKDIR/RESULTS.md"

echo "done -> $WORKDIR/RESULTS.md"
