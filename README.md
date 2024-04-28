Makes a linked list tree, for each item start at the top of the tree and traverse left if it is smaller than that value and right if it is larger
if you traverse right you will find a larger number
if you traverse left you will find a smaller number
traverse down the right path until you find the end (no more larger)
then load up vars back up the list until you find a forked node.
then go down the left once then go right as far as you can again and repeat
nodes with no valid values just go up the the previous node
the list will be sorted biggest to smallest but REALLY slowly
