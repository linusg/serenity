load("test-common.js");

try {
    var bigint = 123n;

    assert(typeof bigint === "bigint");
    assert(-bigint === -123n);

    assertThrowsError(() => {
        +bigint;
    }, {
        error: TypeError,
        message: "Can't convert BigInt to number"
    });

    assert((1n == 1n) === true);
    assert((1n == 1) === true);
    assert((1 == 1n) === true);
    assert((1n == 1.23) === false);
    assert((1.23 == 1n) === false);

    assert((1n != 1n) === false);
    assert((1n != 1) === false);
    assert((1 != 1n) === false);
    assert((1n != 1.23) === true);
    assert((1.23 != 1n) === true);

    assert((1n === 1n) === true);
    assert((1n === 1) == false);
    assert((1 === 1n) === false);
    assert((1n === 1.23) === false);
    assert((1.23 === 1n) === false);

    assert((1n !== 1n) === false);
    assert((1n !== 1) === true);
    assert((1 !== 1n) === true);
    assert((1n !== 1.23) === true);
    assert((1.23 !== 1n) === true);

    assert(1n > 1 === false);
    assert(1 > 1n === false);
    assert(1n > 1 === false);
    assert((1n > 1.23) === false);
    assert((1.23 > 1n) === true);

    assert(1n < 1n === false);
    assert(1n < 1 === false);
    assert(1 < 1n === false);
    assert((1n < 1.23) === true);
    assert((1.23 < 1n) === false);

    assert(1n >= 1n === true);
    assert(1n >= 1 === true);
    assert(1 >= 1n === true);
    assert((1n >= 1.23) === false);
    assert((1.23 >= 1n) === true);

    assert(1n <= 1n === true);
    assert(1n <= 1 === true);
    assert(1 <= 1n === true);
    assert((1n <= 1.23) === true);
    assert((1.23 <= 1n) === false);

    console.log("PASS");
} catch (e) {
    console.log("FAIL: " + e);
}
