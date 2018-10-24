#!/usr/bin/env groovy

def uploadArtifacts(filePaths, uploadPath, artifactServers=['nexus.iroha.tech']) {
  def filePathsConverted = []
  agentType = sh(script: 'uname', returnStdout: true).trim()
  filePaths.each {
    fp = sh(script: "ls -d ${it} | tr '\n' ','", returnStdout: true).trim()
    filePathsConverted.addAll(fp.split(','))
  }
  def shaSumBinary = 'sha256sum'
  def md5SumBinary = 'md5sum'
  def gpgKeyBinary = 'gpg --armor --detach-sign --no-tty --batch --yes --passphrase-fd 0'
  if (agentType == 'Darwin') {
    shaSumBinary = 'shasum -a 256'
    md5SumBinary = 'md5 -r'
    gpgKeyBinary = 'GPG_TTY=\$(tty) gpg --pinentry-mode loopback --armor --detach-sign --no-tty --batch --yes --passphrase-fd 0'
  }
  sh "> \$(pwd)/batch.txt"

  withCredentials([file(credentialsId: 'ci_gpg_privkey', variable: 'CI_GPG_PRIVKEY'), string(credentialsId: 'ci_gpg_masterkey', variable: 'CI_GPG_MASTERKEY')]) {
    if (!agentType.contains('MSYS_NT')) {
      sh "gpg --yes --batch --no-tty --import ${CI_GPG_PRIVKEY} || true"
    }
    filePathsConverted.each {
      sh "echo ${it} >> \$(pwd)/batch.txt;"
      sh "$shaSumBinary ${it} | cut -d' ' -f1 > \$(pwd)/\$(basename ${it}).sha256"
      sh "$md5SumBinary ${it} | cut -d' ' -f1 > \$(pwd)/\$(basename ${it}).md5"
      // TODO @bakhtin 30.05.18 IR-1384. Make gpg command options and paths compatible with Windows OS.
      if (!agentType.contains('MSYS_NT')) {
        sh "echo \"${CI_GPG_MASTERKEY}\" | $gpgKeyBinary -o \$(pwd)/\$(basename ${it}).ascfile ${it}"
        sh "echo \$(pwd)/\$(basename ${it}).ascfile >> \$(pwd)/batch.txt;"
      }
      sh "echo \$(pwd)/\$(basename ${it}).sha256 >> \$(pwd)/batch.txt;"
      sh "echo \$(pwd)/\$(basename ${it}).md5 >> \$(pwd)/batch.txt;"
    }
  }

  withCredentials([usernamePassword(credentialsId: 'ci_nexus', passwordVariable: 'NEXUS_PASS', usernameVariable: 'NEXUS_USER')]) {
    artifactServers.each {
      sh(script: "while read line; do curl -u ${NEXUS_USER}:${NEXUS_PASS} --upload-file \$line https://${it}/repository/artifacts/${uploadPath}/ ; done < \$(pwd)/batch.txt")
    }
  }
}

def hashSum(String binaryPath, String filePath) {
  sum = sh(script: "${binaryPath} ${filePath} | cut -d' ' -f1", returnStdout: true).trim()
  return sum
}

def md5SumLinux(String filePath) {
  return hashSum("md5sum", filePath)
}

def md5SumMac(String filePath) {
  return hashSum("md5 -r", filePath)
}

def sha256SumLinux(String filePath) {
  return hashSum("sha256sum", filePath)
}

def sha256SumMac(String filePath) {
  return hashSum("shasum -a 256", filePath)
}

def writeStringIntoFile(String string, String outFile) {
  sh("echo ${string} > ${outfile}")
  return outFile
}

def gpgDetachedSignatureLinux(String filePath, String outFile, String privateKeyCredentialsID, String masterKeyCredentialsID) {
  withCredentials([file(credentialsId: "${privateKeyCredentialsID}", variable: 'CI_GPG_PRIVKEY'),
    string(credentialsId: "${masterKeyCredentialsID}", variable: 'CI_GPG_MASTERKEY')]) {
      sh "gpg --yes --batch --no-tty --import ${CI_GPG_PRIVKEY} || true"
      gpgKeyBinary = "gpg --armor --detach-sign --no-tty --batch --yes --passphrase-fd 0"
      signatureOutPath = filePath + '.ascfile'
      sh "echo \"${CI_GPG_MASTERKEY}\" | $gpgKeyBinary -o ${signatureOutPath} ${filePath}"
  }
  return signatureOutPath
}

def gpgDetachedSignatureMac(String filePath, String privateKeyCredentialsID, String masterKeyCredentialsID) {
  withCredentials([file(credentialsId: "${privateKeyCredentialsID}", variable: 'CI_GPG_PRIVKEY'),
    string(credentialsId: "${masterKeyCredentialsID}", variable: 'CI_GPG_MASTERKEY')]) {
      sh "gpg --yes --batch --no-tty --import ${CI_GPG_PRIVKEY} || true"
      gpgKeyBinary = "GPG_TTY=\$(tty) gpg --pinentry-mode loopback --armor --detach-sign --no-tty --batch --yes --passphrase-fd 0"
      signatureOutPath = filePath + '.ascfile'
      sh "echo \"${CI_GPG_MASTERKEY}\" | $gpgKeyBinary -o ${signatureOutPath} ${filePath}"
  }
  return signatureOutPath
}

def fileUploadWithCredentials(String filePath, String credentialsId, String remoteServer) {
  withCredentials([usernamePassword(credentialsId: "${credentialsId}", usernameVariable: 'UPLOAD_USER', passwordVariable: 'UPLOAD_PASS')]) {
    sh(script: "curl -u ${UPLOAD_USER}:${UPLOAD_PASS} --upload-file ${filepath} ${remoteServer}")
  }
  return sh(script: "echo \$(echo ${remoteServer} | tr -d '/')/\$(basename ${filePath})", returnStdout: true).trim()
}

return this
