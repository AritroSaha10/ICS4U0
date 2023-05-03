# -----------------------------------------------------------------------------
# Name:        Tower of Hanoi Solver
# Purpose:     Solves the Tower of Hanoi puzzle using a recursive 
#               approach. It also allows you to customize the problem 
#               itself by specifying the number of disks, where they are, 
#               and where they should be moved.
#
# Author:      Aritro Saha
# Created:     09-Apr-2023
# Updated:     10-Apr-2023
# -----------------------------------------------------------------------------


class MoveNotPossibleException(Exception):
    """
    Raised when a Tower of Hanoi disc move is not possible.
    """

    pass


# Rods go from bottom to top
rods: dict[str, list[int]] = {"A": [], "B": [], "C": []}

# Counts how many moves were made in total
total_moves = 0


def move_disc(from_rod: str, to_rod: str, see_moves: bool = True) -> None:
    """
    Moves the top disc of one rod to another, while making sure that the move is possible and follows the rules of Tower of Hanoi.

    Parameters
    ------------
    from_rod : str
        The rod to move the top disc from
    to_rod : str
        The rod to move the top disc to

    Raises
    -------
    MoveNotPossibleException
        If the disc move desired is not possible.
    """
    global total_moves

    if from_rod not in rods or to_rod not in rods:
        # Can't move a disc on a non-existent rod
        raise MoveNotPossibleException(f"from_rod or to_rod does not exist")

    if len(rods[from_rod]) == 0:
        # Can't move a non-existent disc
        raise MoveNotPossibleException(f"Disc does not exist on rod {from_rod}")

    # Get and remove the topmost disc from its rod
    disc_to_move = rods[from_rod].pop()
    if len(rods[to_rod]) > 0 and disc_to_move > rods[to_rod][-1]:
        # There are disc(s) on the to_rod that are smaller than the disc we'd like to move, which would be an illegal move
        raise MoveNotPossibleException(
            f"Top disc on rod {from_rod} cannot be moved on top of rod {to_rod}"
        )

    # Move the disc onto the desired rod
    rods[to_rod].append(disc_to_move)

    # Log the move in the console
    if see_moves:
        print(
            f"Step {total_moves+1}: Move disc {disc_to_move} from rod {from_rod} to {to_rod}"
        )
        print_towers()
        print()

    # Increment move counter
    total_moves += 1


def print_towers() -> None:
    """
    Print the current state of the Tower of Hanoi rods.
    """
    for rod in rods:
        print(f"{rod}: {' '.join([str(x) for x in rods[rod]])}")


def solve_tower_of_hanoi(
    disc_to_move: int, from_rod: str, to_rod: str, see_moves: bool
) -> None:
    """
    Recursively solve a Tower of Hanoi puzzle.

    Parameters
    ------------
    disc_to_move : int
        The disc to move.
    from_rod : str
        The name of the rod that holds the disc to move.
    to_rod : str
        The name of the rod that the disc should be moved to, following the puzzle's rules.
    see_moves : bool
        Whether the moves should be printed to the console
    """

    if disc_to_move == 0:
        # There is no disc to move, so this becomes the end of the recursion stack.
        return

    # Since the Tower of Hanoi puzzle only ever has three rods, the auxilary rod (spare rod used to move the discs to the desired rod) can only be the rod that isn't the from or to rod
    aux_rod = set(rods.keys()).difference(set([from_rod, to_rod])).pop()

    # Move the discs on top of the disc_to_move onto the auxilary rod
    solve_tower_of_hanoi(
        disc_to_move=disc_to_move - 1,
        from_rod=from_rod,
        to_rod=aux_rod,
        see_moves=see_moves,
    )

    # Move the disc_to_move onto the desired rod (to_rod)
    move_disc(from_rod=from_rod, to_rod=to_rod, see_moves=see_moves)

    # Move the discs we moved onto the auxilary rod back on top of the disc_to_move
    solve_tower_of_hanoi(
        disc_to_move=disc_to_move - 1,
        from_rod=aux_rod,
        to_rod=to_rod,
        see_moves=see_moves,
    )


if __name__ == "__main__":
    # Get the # of discs that we should start with
    disc_count = -1

    # Constantly ask the user for valid input until they actually give a proper answer
    while disc_count == -1 and (
        (tmp := input("How many discs would you like for the puzzle? ")) or tmp == ""
    ):
        if tmp.isdigit() and int(tmp) > 0:
            disc_count = int(tmp)
        else:
            print("Please provide a valid number of discs.\n")

    print()

    # Get the name of the rod that these discs should start on
    starting_rod = ""
    while starting_rod == "" and (
        tmp := input("What rod should the discs start on? (A, B, C) ")
    ):
        if tmp in ["A", "B", "C"]:
            starting_rod = tmp
        else:
            print("Please provide a valid rod.\n")

    print()

    # Generate a list of the discs from greatest to least
    rods[starting_rod] = [i for i in range(disc_count, 0, -1)]

    # Display the starting state of the rods
    print("Starting state of rods:")
    print_towers()
    print()

    # Find the disc that should be moved
    disc_to_move = -1
    while disc_to_move == -1 and (
        (tmp := input("What disc should be moved? ")) or tmp == ""
    ):
        if tmp.isdigit() and (tmp := int(tmp)) > 0 and tmp <= disc_count:
            disc_to_move = tmp
        else:
            print("Please provide a valid disc to move.\n")

    print()

    # Find where the disc and those above it should move to
    to_rod = ""
    while to_rod == "" and (
        (
            tmp := input(
                "What rod should the disc and those above it end up on? (A, B, C) "
            )
        )
        or tmp == ""
    ):
        if tmp in ["A", "B", "C"]:
            to_rod = tmp
        else:
            print("Please provide a valid rod.\n")

    print()

    # Check whether user would like to see the moves
    see_moves = None
    while see_moves == None and (
        (tmp := input("Would you like to see the moves? (y/n) ")) or tmp == ""
    ):
        if tmp == "y":
            see_moves = True
        elif tmp == "n":
            see_moves = False
        else:
            print("Please provide a valid answer.\n")

    print()

    # Solve the Tower of Hanoi puzzle given the user setup
    solve_tower_of_hanoi(disc_to_move, starting_rod, to_rod, see_moves)

    print(
        f"It takes {total_moves} to solve the Tower of Hanoi puzzle with the given constraints."
    )
