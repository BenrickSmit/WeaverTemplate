#!/bin/sh
# This is a wrapper script. The core logic is in the scripts directory.
"$(dirname "$0")/scripts/build-core.sh" "$@"