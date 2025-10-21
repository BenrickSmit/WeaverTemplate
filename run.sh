#!/bin/sh
# This is a wrapper script. The core logic is in the scripts directory.
"$(dirname "$0")/run_workflow.sh" "$@"