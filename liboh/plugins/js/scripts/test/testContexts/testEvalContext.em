//tests whether system eval works.  Should print 3 after done.

system.print("\n\nShould be able to see that x=3 after this.\n\n");


system.import('test/testContexts/baseContextTest.em');

function testContext()
{
    newContext.execute(toExecute);
}


function toExecute()
{
    system.eval('x=3');
    system.print("\n\n--");
    system.print(x);
    system.print("--\n\n");
}

testContext();