import jp.co.soramitsu.iroha.*

fun main(args: Array<String>) {
    System.loadLibrary("irohajava")
    val crypto = ModelCrypto()
    val keypair = crypto.generateKeypair()

    val (priv, pub) = listOf(keypair.privateKey(), keypair.publicKey()).map { it.hex() }
    println("$priv;$pub")
}


