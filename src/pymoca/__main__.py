import logging
import sys

from pymoca.compiler import main

if __name__ == "__main__":
    err = main(sys.argv[1:])
    logging.shutdown()
    sys.exit(err)
